#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

#include "typedef.h"
#include "vector.h"
#include "string8.h"

#define INPUT_MAX_LEN 255
#define PATH_MAX 1024

vector split(s8 *str, const char splited){
  char c; 
  u64 idx = 0;
  i64 prev = -1;
  u64 len = s8_len(str);
  vector ret = vector_create(s8, 0, NULL);
  while(idx < len){
    char c = s8_char(str, idx);
    if(c == splited || c == '\0'){
      s8 sub = s8_substr(str, prev+1, idx);
      vector_append(&ret, &sub);

      if(c == '\0'){
        break;
      }
      prev = idx;
    }
    ++idx;
  }
  return ret;
}

b8 path_of_command(s8 *poc, s8 *cmd){
  const char *env_var_val = getenv("PATH");
  s8 paths_str = s8_create(env_var_val, strlen(env_var_val));
  vector paths = split(&paths_str, ':');
  for(int i = 0; i < vector_size(&paths); ++i){
    s8 *p = vector_at(s8, &paths, i);

    DIR *dirp = opendir(p->str);
    if(dirp == NULL){
      return false;
    }
    for(;;){
      struct dirent *dp = readdir(dirp);
      if(dp == NULL){
        break;
      }

      if(strcmp(dp->d_name, cmd->str) != 0){
        continue;
      }

      struct stat sb;
      s8_append(p, '/');
      s8 path = s8_strcat(p, cmd);
      if(stat(path.str, &sb) == -1){
        continue;
      }
      if(S_ISREG(sb.st_mode) && (sb.st_mode & 0x49) == 0x49){
        memcpy(poc, &path, sizeof(s8));
        break;
      }
    }
    closedir(dirp);
    if(s8_len(poc) > 0){
      return true;
    }
  }
  return false;
}
void type_cb(vector *components){
  s8 *cmd = vector_at(s8, components, 1);
  s8 poc = {
    .len = 0,
    .size = 0,
    .capacity = 0
  }; //path of command

  if(!strcmp(s8_string(cmd), "echo")){
    printf("echo is a shell builtin\n");
  }else if(!strcmp(s8_string(cmd), "type")){
    printf("type is a shell builtin\n");
  }else if(!strcmp(s8_string(cmd), "exit")){
    printf("exit is a shell builtin\n");
  }else if(!strcmp(s8_string(cmd), "pwd")){
    printf("pwd is a shell builtin\n");
  }else if(path_of_command(&poc, cmd)){
    printf("%s is %s\n", cmd->str, poc.str);
  }else{
    printf("%s: not found\n", s8_string(cmd));
  }
}

void echo_cb(vector *components){
  for(u64 i = 1; i < vector_size(components); ++i){
    s8 *s = vector_at(s8, components, i);
    printf("%s ", s8_string(s));
  }
  printf("\n");
}

void pwd_cb(vector *components){
  char cwd[PATH_MAX];
  getcwd(cwd, PATH_MAX);
  printf("%s\n", cwd);
}

void cd_cb(vector *components){
  s8 dict;
  s8 *argv = vector_at(s8, components, 1);
  if(s8_char(argv, 0) == '~'){
    s8_erase(argv, 0, 1);
    const char *env_var_val = getenv("HOME");
    s8 home_path = s8_create(env_var_val, strlen(env_var_val));
    dict = s8_strcat(&home_path, argv);
    s8_destroy(&home_path);
  }else{
    dict = *argv;
  }
  i64 ret = chdir(dict.str);
  if(ret < 0){
    if(errno == ENOENT){
      printf("cd: %s: No such file or directory\n", s8_string(&dict));
    }
  }
}

void (*cmd_lookup(s8 *s))(vector *components) {
  if(!strcmp(s8_string(s), "echo")){
    return &echo_cb;
  }
  if(!strcmp(s8_string(s), "type")){
    return &type_cb;
  }
  if(!strcmp(s8_string(s), "pwd")){
    return &pwd_cb;
  }
  if(!strcmp(s8_string(s), "cd")){
    return &cd_cb;
  }
  return NULL;
}

void cleanup(vector *vec_s8, s8 *s){
  for(u64 i = 0; i < vector_size(vec_s8); ++i){
    s8 *s = vector_at(s8, vec_s8, i);
    s8_destroy(s);
  }
  vector_destroy(vec_s8);
  s8_destroy(s);
}

void doREPL(){
  char user_input[INPUT_MAX_LEN];
  while(1){
    printf("$ ");
    fgets(user_input, INPUT_MAX_LEN, stdin);
    user_input[strcspn(user_input, "\n")] = '\0';

    s8 input_str = s8_create(user_input, INPUT_MAX_LEN);
    vector components = split(&input_str, ' ');
    if(vector_size(&components) == 0){
      cleanup(&components, &input_str);
      continue;
    }

    s8 *cmd = vector_at(s8, &components, 0);
    if(!strcmp(s8_string(cmd), "exit")){
      cleanup(&components, &input_str);
      break;
    }

    s8 poc;
    void (*callback)(vector *components) = cmd_lookup(cmd);
    if(callback){
      callback(&components);
    }else if(path_of_command(&poc, cmd)){
      system(user_input);
    }else{
      printf("%s: command not found\n", s8_string(cmd));
    }
    cleanup(&components, &input_str);
  }
}

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  doREPL();
  return 0;
}
