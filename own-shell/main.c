#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "typedef.h"

#define PATH_NO_MAX           15
#define COMMAND_BUFFER_MAX    255
#define PATH_BUFFER_MAX       255

typedef enum {
  E_SUCESS = 0,
  E_INPUT_TOO_LONG,
  E_EOF
} ErrorCode;

typedef struct {
  char *name;
  void (*handler)(i32 argc, char *argv[]);
} Buildin;

static void buildin_type(i32 argc, char *argv[]);
static void buildin_pwd(i32 argc, char *argv[]);
static void buildin_echo(i32 argc, char *argv[]);
static void buildin_cd(i32 argc, char *argv[]);
static void buildin_exit(i32 argc, char *argv[]);

static u32 string_length(const char *s){
  u32 idx = 0;
  while(s[idx] != '\0'){
    ++idx;
  }
  return idx;
}

static i32 string_compare(const char *s1, const char *s2){
  i32 ret = 0;
  i32 idx = 0;
  while(s1[idx] == s2[idx]){
    if(s1[idx] == '\0' || s2[idx] == '\0'){
      break;
    }
    ++idx;
  }
  ret = s1[idx] - s2[idx];
  return ret;
}

static char *string_concate(char *dest, char *src){
  u32 dest_len = string_length(dest);
  u32 src_len = string_length(src);

  memcpy(dest + dest_len, src, src_len);
  return dest;
}

i32 find_buildin_index(char *command_name);

b8          is_running = true;
b8          is_redirect = false;
b8          is_redirect_append = false;
char       *prompt = "$ ";
char       *redirect_to;
char       *redirect_from;
char        command[COMMAND_BUFFER_MAX];
char       *arguments[COMMAND_BUFFER_MAX];
ErrorCode   error_code = E_SUCESS;

Buildin buildin_commands[] = {
  {.name = "type", .handler = buildin_type},
  {.name = "pwd", .handler = buildin_pwd},
  {.name = "cd", .handler = buildin_cd},
  {.name = "echo", .handler = buildin_echo},
  {.name = "exit", .handler = buildin_exit}
};

void end_program(void){
  is_running = false;
}

static i32 split(char *str, u32 len, char split_char, char *strlist[]){
  char c;
  u32 strno = 0;
  u32 idx = 0;
  while((c = str[idx]) != '\n' && c != '\0'){
    if(idx > len){
      return -1;
    }
    if(c == split_char){
      str[idx] = '\0';
    }else {
      if(strno == 0 || str[idx-1] == '\0'){
        strlist[strno++] = str + idx;
      }
    }
    ++idx;
  }
  return strno;
}

static i32 search_program(char *program, u32 size1, char *entered_command, u32 size2){
  char *path_list[PATH_NO_MAX];
  const char *env_paths = getenv("PATH");
  u32 env_paths_len = string_length(env_paths);
  char env_paths_clone[env_paths_len+1];

  memset(env_paths_clone, 0x00, env_paths_len+1);
  memcpy(env_paths_clone, env_paths, env_paths_len);

  i32 ret = split(env_paths_clone, env_paths_len, ':', path_list);
  if(ret < 1){
    return ret;
  }

  for(i32 i = 0; i < ret; ++i){
    DIR *dirp = opendir(path_list[i]);
    if(dirp == NULL){
      return -1;
    }
    for(;;){
      struct dirent *dp = readdir(dirp);
      if(dp == NULL){
        break;
      }

      if(string_compare(dp->d_name, entered_command) != 0){
        continue;
      }

      struct stat sb;
      char path[PATH_BUFFER_MAX];
      u32 path_len = string_length(path_list[i]);
      u32 cmd_len = string_length(entered_command);

      memset(path, 0x00, PATH_BUFFER_MAX);

      if(path_len + cmd_len >= PATH_BUFFER_MAX - 2){
        error_code = E_INPUT_TOO_LONG;
        return -1;
      }
      
      memcpy(path, path_list[i], path_len);
      path[path_len] = '/';
      memcpy(path + path_len + 1, entered_command, cmd_len);

      if(stat(path, &sb) == -1){
        continue;
      }
      if(S_ISREG(sb.st_mode) && (sb.st_mode & 0x49) == 0x49){
        memcpy(program, path, MIN(size1, PATH_BUFFER_MAX));
        return 1;
      }
    }
  }
  return -1;
}

static void buildin_exit(i32 argc, char *argv[]){
  end_program();
}

static void buildin_type(i32 argc, char *argv[]){
  char *buildin_command = argv[1];
  i32 buildin_idx = find_buildin_index(buildin_command);
  if(buildin_idx >= 0){
    printf("%s is a shell builtin\n", buildin_command);
    return;
  }
  char program[PATH_BUFFER_MAX];
  if(search_program(program, PATH_BUFFER_MAX, buildin_command, string_length(buildin_command)) == 1){
    printf("%s is %s\n", buildin_command, program);
    return;
  }
  printf("%s: not found\n", buildin_command);
}

static void buildin_pwd(i32 argc, char *argv[]){
  char cwd[PATH_BUFFER_MAX];
  getcwd(cwd, PATH_BUFFER_MAX);
  printf("%s\n", cwd);
}

static char *get_parent_dir(char *parent, char *path){
  u32 path_len = string_length(path);
  u32 idx = path_len-1;
  if(path_len < 1){
    return NULL;
  }

  while(path[idx] != '/' && idx > 0){
    --idx;
  }

  if(idx > 0){
    memcpy(parent, path, idx);
  }
  return parent;
}

static void prepare_directory(char *dest){
  char parent_dir[PATH_BUFFER_MAX];
  memset(parent_dir, 0x00, PATH_BUFFER_MAX);
  get_parent_dir(parent_dir, dest);
  i32 fd = open(parent_dir, O_RDONLY | O_DIRECTORY);
  if(fd == -1){
    mkdir(parent_dir, 0700);
  }else{
    close(fd);
  }
}

static void buildin_echo(i32 argc, char *argv[]){
  u32 content_len = 0;
  b8 is_content = true;
  char content[COMMAND_BUFFER_MAX];
  u32 path_len = 0;
  char path[PATH_BUFFER_MAX];

  memset(content, 0x00, COMMAND_BUFFER_MAX);
  memset(path, 0x00, PATH_BUFFER_MAX);

  for(i32 i = 1; i < argc; ++i){
    string_concate(content, argv[i]);
    content_len = string_length(content);
    content[content_len] = ' ';
  }

  content[content_len] = '\n';
  ++content_len;
  
  if(redirect_to != NULL && redirect_from == NULL && string_length(redirect_to) > 0){
    prepare_directory(redirect_to);
    i32 fd;
    if(is_redirect_append){
      fd = open(redirect_to, O_WRONLY | O_CREAT | O_APPEND, 0700);
    }else{
      fd = open(redirect_to, O_WRONLY | O_CREAT, 0700);
    }
    write(fd, content, content_len);
    close(fd);
    return;
  }
  write(STDOUT_FILENO, content, content_len);
}

static void preprocess(char *cleaned_dir, char *entered_dir){
  u32 offset = 0;
  u32 min_len = 0;
  char *copy_pos = entered_dir;
  if(copy_pos[0] == '~'){
    const char *home_dir = getenv("HOME");
    min_len = MIN(PATH_BUFFER_MAX-1, string_length(home_dir));
    memcpy(cleaned_dir, home_dir, min_len);
    offset = min_len;
    ++copy_pos;
  }
  min_len = MIN(PATH_BUFFER_MAX-1, string_length(entered_dir) + offset);
  memcpy(cleaned_dir + offset, copy_pos, min_len);
  cleaned_dir[min_len] = '\0';
}

static void buildin_cd(i32 argc, char *argv[]){
  char *entered_dir = argv[1];
  char cleaned_dir[PATH_BUFFER_MAX];

  preprocess(cleaned_dir, entered_dir);
  i64 ret = chdir(cleaned_dir);
  if(ret < 0){
    if(errno == ENOENT){
      printf("cd: %s: No such file or directory\n", cleaned_dir);
    }
  }
}

static void drain_stdin(void){
  i32 c;
  while((c = getchar()) != '\n' && c != EOF)
    ;
}

string_match(char *s1, char *s2, u32 length){
  for(u32 i = 0; i < length; ++i){
    if(s1[i] != s2[i]){
      return false;
    }
  }
  return true;
}

i32 find_matching_command(char *suggestions[], char *input, u32 length){
  i32 match_count = 0;
  for(i32 i = 0; i < sizeof(buildin_commands)/sizeof(Buildin); ++i){
    if(string_match(buildin_commands[i].name, input, length)){
      suggestions[match_count++] = buildin_commands[i].name;
    }
  }
  return match_count;
}

void suggest_command(char *input, u32 length){
  char *suggestions[sizeof(buildin_commands)/sizeof(Buildin)];

  i32 match_count = find_matching_command(suggestions, input, length);

  if(match_count == 0){
    return;
  }

  if(match_count == 1){
    printf("\r\033[K%s%s", prompt, suggestions[0]);
    return;
  }else{
    printf("\n");
    for(i32 i = 0; i < match_count; ++i){
      printf("%s\n", suggestions[i]);
    }
    printf("\r\033[K%s%s", prompt, input);
  }
}

i32 read_command(void){
  i32 c;
  char in_space = ' ';
  i32 argc = 0;
  u32 idx = 0;
  b8 is_quotes = false;
  b8 is_double_quotes = false;
  b8 is_backslash = false;

  memset(command, 0x00, sizeof(command));
  memset(arguments, 0x00, sizeof(arguments));
  while((c = getchar()) != '\n'){
    if(c == EOF){
      error_code = E_EOF;
      return -1;
    }

    if(c == '\t'){
      suggest_command(command, idx);
      continue;
    }

    if(idx >= COMMAND_BUFFER_MAX - 1){
      error_code = E_INPUT_TOO_LONG;
      return -1;
    }

    if(is_backslash){
      command[idx++] = (char) c;
      is_backslash = false;
      continue;
    }

    if(c == in_space && is_quotes == false && is_double_quotes == false){
      command[idx++] = '\0';
      continue;
    }

    if(c == '\"' && is_quotes == false){
      is_double_quotes = !is_double_quotes;
      continue;
    }

    if(c == '\'' && is_double_quotes == false){
      is_quotes = !is_quotes;
      continue;
    }

    if(c == '>' && is_quotes == false && is_double_quotes == false && is_backslash == false){
      if(is_redirect){
        is_redirect_append = true;
      }else{
        is_redirect = true;
        if(idx > 0 && command[idx-1] != '\0'){
          --argc;
          redirect_from = arguments[argc];
          arguments[argc] = NULL;
        }
      }
      command[idx++] = (char) c;
      continue;
    }
  
    if(argc == 0 || command[idx-1] == '\0'){
      if(is_redirect){
        redirect_to = command + idx;
        is_redirect = false;
      }else{
        arguments[argc++] = command + idx;
      }
    }

    if(c == '\\' && is_quotes == false){
      is_backslash = true;
      continue;
    }

    command[idx++] = (char) c;
  }

  return argc;
}

void handle_error(void){
  switch (error_code)
  {
  case E_INPUT_TOO_LONG:
    puts("sh: Command too long");
    drain_stdin();
    break;
  
  case E_EOF:
    end_program();
    break;
  
  default:
    break;
  }
  error_code = E_SUCESS;
}

i32 find_buildin_index(char *command_name){
  for(i32 i = 0; i < sizeof(buildin_commands)/sizeof(Buildin); ++i){
    if(!string_compare(command_name, buildin_commands[i].name))
    {
      return i;
    }
  }
  return -1;
}

void execute_command(i32 argc, char *argv[]){
  if(argc < 1){
    return;
  }

  i32 redirect_from_int;
  i32 saved_direct = -1;
  i32 buildin_idx = find_buildin_index(argv[0]);
  if(buildin_idx >= 0){
    i32 fd = -1;
    if(redirect_from != NULL && redirect_to != NULL){
      prepare_directory(redirect_to);
      if(is_redirect_append){
        fd = open(redirect_to, O_WRONLY | O_CREAT | O_APPEND, 0700);
      }else{
        fd = open(redirect_to, O_WRONLY | O_CREAT, 0700);
      }
      redirect_from_int = redirect_from[0] - '0';
      saved_direct = dup(redirect_from_int);
      dup2(fd, redirect_from_int);
    }
    buildin_commands[buildin_idx].handler(argc, argv);
    if(fd > -1){
      close(fd);
    }
    if(saved_direct != -1){
      dup2(saved_direct, redirect_from_int);
    }
    return;
  }

  char program[PATH_BUFFER_MAX];
  i32 ret = search_program(program, PATH_BUFFER_MAX, argv[0], string_length(argv[0]));
  if(program && ret == 1){
    pid_t pid = fork();
    if(pid == 0){
      i32 fd = -1;
      if(redirect_to != NULL){
        prepare_directory(redirect_to);
        if(is_redirect_append){
          fd = open(redirect_to, O_WRONLY | O_CREAT | O_APPEND, 0700);
        }else{
          fd = open(redirect_to, O_WRONLY | O_CREAT, 0700);
        }
        redirect_from_int = 1;
        if(redirect_from != NULL){
          redirect_from_int = redirect_from[0] - '0';
        }
        dup2(fd, redirect_from_int);
      }
      exit(execv(program, arguments));
      if(fd > -1){
        close(fd);
      }
    }else{
      int status;
      waitpid(pid, &status, 0);
    }
  }else{
    printf("%s: command not found\n", argv[0]);
  }
}

void handle_command(void){
  char c;
  printf("%s ", prompt);
  i32 argc = read_command();
  if(argc <= 0){
    handle_error();
    return;
  }

  execute_command(argc, arguments);
  redirect_to = NULL;
  redirect_from = NULL;
  is_redirect = false;
  is_redirect_append = false;
}

void doREPL(void){
  struct termios tp;
  tcgetattr(STDIN_FILENO, &tp);
  struct termios tp_old = tp;
  tp.c_lflag &= ~ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &tp);
  while(is_running){
    handle_command();    
  }
  tcsetattr(STDIN_FILENO, TCSANOW, &tp_old);
}

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  doREPL();
  return 0;
}
