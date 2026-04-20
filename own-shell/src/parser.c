#include "parser.h"
#include "utils.h"

/**
 * Helper function to detect redirect operator type
 * Returns: REDIRECT_MODE_APPEND (>>), REDIRECT_MODE_TRUNCATE (>), or -1 (not a redirect)
 */
static i32 detect_redirect_mode(char *arg){
  if(arg == NULL || arg[0] == '\0'){
    return -1;
  }
  
  if(arg[0] == '>' && arg[1] == '>' && arg[2] == '\0'){
    return REDIRECT_MODE_APPEND;
  }
  if(arg[0] == '>' && arg[1] == '\0'){
    return REDIRECT_MODE_TRUNCATE;
  }
  
  return -1;
}

/**
 * Helper function to extract file descriptor from argument
 * Examples: "1", "2", "1>", "2>>", etc.
 * Returns: file descriptor number, or -1 if not a number
 */
static i32 extract_fd(char *arg, i32 *out_fd){
  if(arg == NULL || arg[0] == '\0'){
    return -1;
  }
  
  // Find where the redirect operator starts
  char *redirect_pos = find_last_char_in_string(arg, '>');
  
  if(redirect_pos == NULL){
    return -1;  // No redirect operator
  }
  
  // If redirect is at the beginning, default to stdout (fd 1)
  if(redirect_pos == arg){
    *out_fd = STDOUT_FILENO;
    return 0;
  }
  
  // Extract digits before redirect operator
  u32 fd_len = redirect_pos - arg;
  if(fd_len >= 10){
    return -1;  // Too long for a file descriptor
  }
  
  char fd_str[10];
  strncpy(fd_str, arg, fd_len);
  fd_str[fd_len] = '\0';
  
  if(!is_numeric_string(fd_str)){
    return -1;  // Not a number
  }
  
  *out_fd = atoi(fd_str);
  return 0;
}

/**
 * Helper function to add a redirect to the command
 */
static i32 add_redirect(Command *cmd_ptr, i32 fd, char *file, RedirectMode mode){
  if(cmd_ptr->redirect.redirect_count >= REDIRECT_FILE_MAX){
    throw_error(E_INVALID_PARAMETER, "Too many redirects");
    return -1;
  }
  
  cmd_ptr->redirect.redirect_fd[cmd_ptr->redirect.redirect_count] = fd;
  cmd_ptr->redirect.redirect_file[cmd_ptr->redirect.redirect_count] = file;
  cmd_ptr->redirect.mode[cmd_ptr->redirect.redirect_count] = mode;
  cmd_ptr->redirect.redirect_count++;
  
  return 0;
}

/**
 * Main parser function - converts raw input into Command structure
 * Handles quotes, escapes, and redirects (> and >>)
 */
i32 parse_command(char *raw, u32 raw_len, Command *cmd_ptr){
  char c;
  char *arguments[ARGC_MAX];
  u32 argc = 0;
  u32 idx = 0;
  b8 quotes = false;
  b8 dquotes = false;
  b8 backslash = false;

  if(raw_len > IN_LEN){
    throw_error(E_INPUT_TOO_LONG, "Input is longer than command buffer");
    return -1;
  }

  // Initialize command structure
  cmd_ptr->argc = 0;
  cmd_ptr->redirect.redirect_count = 0;

  // Parse input character by character
  for(u32 raw_idx = 0; raw_idx < raw_len; ++raw_idx){
    if((c = raw[raw_idx]) == '\0'){
      break;
    }
    
    // Skip newlines
    if(c == '\n'){
      continue;
    }

    // Handle escape sequences
    if(c == '\\' && backslash == false){
      backslash = true;
      continue;
    }
    
    // Handle single quotes
    if(c == '\'' && dquotes == false && backslash == false){
      quotes = !quotes;
      continue;
    }

    // Handle double quotes
    if(c == '\"' && quotes == false && backslash == false){
      dquotes = !dquotes;
      continue;
    }

    // Handle spaces (argument separator)
    if(c == ' ' && quotes == false && dquotes == false && backslash == false){
      cmd_ptr->buffer[idx++] = '\0';
      continue;
    }

    // Handle redirect operators
    if(c == '>' && quotes == false && dquotes == false && backslash == false){
      // Null-terminate current argument if any
      if(idx > 0 && cmd_ptr->buffer[idx-1] != '\0'){
        cmd_ptr->buffer[idx++] = '\0';
      }
      
      // Add redirect operator marker
      arguments[argc++] = cmd_ptr->buffer + idx;
      cmd_ptr->buffer[idx++] = c;
      
      // Look ahead for second '>' (for >>)
      if(raw_idx + 1 < raw_len && raw[raw_idx + 1] == '>'){
        cmd_ptr->buffer[idx++] = '>';
        raw_idx++;  // Skip the next '>'
      }
      
      cmd_ptr->buffer[idx++] = '\0';
      continue;
    }

    // Mark start of new argument
    if(argc == 0 || cmd_ptr->buffer[idx-1] == '\0'){
      arguments[argc++] = cmd_ptr->buffer + idx;
    }
    
    cmd_ptr->buffer[idx++] = c;

    if(backslash){
      backslash = false;
    }
  }

  // Null-terminate the last argument
  if(idx > 0 && cmd_ptr->buffer[idx-1] != '\0'){
    cmd_ptr->buffer[idx++] = '\0';
  }

  // Check for unclosed quotes
  if(quotes == true || dquotes == true){
    throw_error(E_INVALID_PARAMETER, "Unclosed quotes");
    return -1;
  }

  // Parse arguments and handle redirects
  for(u32 i = 0; i < argc; ++i){
    // Skip empty arguments
    if(arguments[i][0] == '\0'){
      continue;
    }

    // Check if argument is a redirect operator (> or >>)
    i32 redirect_mode = detect_redirect_mode(arguments[i]);
    if(redirect_mode >= 0){
      // Next argument should be the filename
      if(i + 1 >= argc || arguments[i+1][0] == '\0'){
        throw_error(E_SYNTAX_ERROR, "syntax error: missing filename after redirect");
        return -1;
      }

      // Default to stdout
      i32 fd = STDOUT_FILENO;
      
      // Add redirect
      if(add_redirect(cmd_ptr, fd, arguments[i+1], redirect_mode) < 0){
        return -1;
      }
      
      i++;  // Skip the filename
      continue;
    }

    // Check if argument contains a redirect (e.g., "1>", "2>>")
    i32 fd = -1;
    if(extract_fd(arguments[i], &fd) == 0){
      // This is "FD>" or "FD>>" pattern
      if(i + 1 >= argc || arguments[i+1][0] == '\0'){
        throw_error(E_SYNTAX_ERROR, "syntax error: missing filename after redirect");
        return -1;
      }

      // Determine redirect mode
      char *op = find_last_char_in_string(arguments[i], '>');
      i32 mode = (op[1] == '>') ? REDIRECT_MODE_APPEND : REDIRECT_MODE_TRUNCATE;
      
      if(add_redirect(cmd_ptr, fd, arguments[i+1], mode) < 0){
        return -1;
      }
      
      i++;  // Skip the filename
      continue;
    }

    // Check if this is a file descriptor followed by redirect operator
    // Pattern: "1" ">" "file" or "2" ">" "file"
    if(is_numeric_string(arguments[i]) && i + 1 < argc){
      i32 next_redirect_mode = detect_redirect_mode(arguments[i+1]);
      if(next_redirect_mode >= 0){
        // Next-next argument should be the filename
        if(i + 2 >= argc || arguments[i+2][0] == '\0'){
          throw_error(E_SYNTAX_ERROR, "syntax error: missing filename after redirect");
          return -1;
        }

        // Extract file descriptor
        i32 fd = atoi(arguments[i]);
        
        // Add redirect
        if(add_redirect(cmd_ptr, fd, arguments[i+2], next_redirect_mode) < 0){
          return -1;
        }
        
        i += 2;  // Skip the redirect operator and filename
        continue;
      }
    }

    // Regular argument - add to command
    if(cmd_ptr->argc < ARGC_MAX){
      cmd_ptr->arguments[cmd_ptr->argc++] = arguments[i];
    }
  }

  return 0;
}