#ifndef _COMMAND_H_
#define _COMMAND_H_
#include "node.h"

#include <pthread.h>
#include <string>
#include <sstream>

#define MAX_TOKEN_SIZE 2048

class Planet;

class TextCommand : public Command
{
public:
  TYPED("Mutex.Thread.Command.TextCommand")
  TextCommand(std::istream &input, std::ostream &output) : Command("text"), current_directory_("/"), input_(&input), output_(&output)
  { initialize(); }
  
  TextCommand() : Command("text"), current_directory_("/") {
    input_  = &std::cin;
    output_ = &std::cout;
    initialize();
  }
  
  virtual ~TextCommand() {}
  
  /** Clear the current command. */
  void clear();
  
  /** Ragel parser. */
  void parse(const char *string) {
    parse(std::string(string));
  }
  
  /** Ragel parser. */
  void parse(const std::string &string);
  
  /** Used for testing. */
  void set_input(std::istream &input) { input_ = &input; }
  
  /** Used for testing. */
  void set_output(std::ostream &output) { output_ = &output; }
  
  /** Do not print command results back. */
  void set_silent() { silent_ = true; }
  
  /** Print command results back. */
  void set_verbose() { silent_ = false; }
  
  virtual Object* build_remote_object(const Url &url, Value* error) {
    error->set(BAD_REQUEST_ERROR, "TextCommand cannot create references to remote objects.");
    return NULL;
  }
  
  void print_result(const Value &res) {
    if (!silent_ && !res.is_nil()) *output_ << res << std::endl;
  }
protected:
  /** Constructor, set default values. */
  void initialize();
  
  /** Code executed in a separate thread. Runs until deleted or quit. */
  virtual void do_listen();
  
  /** RAGEL PARSER RELATED CALLBACKS **/
  
  /** Set a variable from the current token content. */
  void set_from_token(std::string &string);
  
  /** Set the class name. */
  void set_class_from_token();
  
  /** Transform node names to absolute urls depending on the current directory. */
  void names_to_urls() {
    from_node_.insert(0, current_directory_);
    to_node_.insert(0,   current_directory_);
    var_.insert(0,   current_directory_);
  }
  
  /** Create an instance. */
  void create_instance();
  
  /** Create a link. If the link cannot be created right now because all variables aren't set yet, the link will be kept in Planet's link buffer until all variables are found. */
  void create_link() {
    change_link('c');
  }
  
  /** Remove a link. */
  void remove_link() {
    change_link('d');
  }
  
  void change_link(char op);
  
  /** Execute a method on an instance. */
  void execute_method();
  
  /** Execute a class method. */
  void execute_class_method();

  /** Execute a command or inspect instance. */
  void execute_command();
  
  /** Read a line from input stream. */
  virtual bool getline(char **buffer, size_t size) {
    if (input_->eof()) return false;
    input_->getline(*buffer, size);
    return true;
  }
  
  /** Save a line in edit history. */
  virtual void saveline(const char *line) {}
  
  /** Free a line. */
  virtual void freeline(char *line) {}
    
  /** Token for the parser. */
  std::string  token_;
  unsigned int current_state_; /**< Current parser state between blocks. */
  
  /** Command building parts. */
  std::string     var_, method_, class_, key_, value_, from_node_, to_node_,;
  std::string     parameter_string_;
  std::string     from_port_, to_port_;
  
  std::string     current_directory_;  /**< Current directory url (used to prefix relative urls). */
  
  /** IO management. */
  std::istream *input_;
  std::ostream *output_;
  
  bool silent_;
};

#ifdef USE_READLINE

extern "C" {
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
}

class CommandLine : public TextCommand
{
public:
  TYPED("Mutex.Thread.Command.TextCommand.CommandLine")
  CommandLine(std::istream &input, std::ostream &output) : TextCommand(input, output) {
    read_history(history_path().c_str());
  }
  
  CommandLine() {
    // read readline history
    read_history(history_path().c_str());
  }
  
  virtual ~CommandLine() {
    // save readline history
    *output_ << "\nBye..." << std::endl;
    write_history(history_path().c_str());
  }
  
  virtual bool getline(char **buffer, size_t size) {
    *buffer = readline("> "); // FIXME: this would not work if input is not stdin...
    return *buffer != NULL;
  }
  
  virtual void saveline(const char *line)
  {
    if (strlen(line) > 0) {
      add_history(line);
    }
  }
  
  virtual void freeline(char *line) {
    free(line);
  }
  
private:
  std::string history_path() {
    std::string home(getenv("HOME"));
    home.append("/.rubyk_history");
    return home;
  }
};

#else

class CommandLine : public TextCommand
{
public:
  TYPED("Mutex.Thread.Command.TextCommand.CommandLine")
  CommandLine(std::istream &input, std::ostream &output) : TextCommand(input, output) {}
  CommandLine() {}
  
  virtual bool getline(char ** buffer, size_t size) {
    *output_ << "> ";
    if (input_->eof()) return false;
    input_->getline(*buffer, size);
    return true;
  }
};

#endif // USE_READLINE

#endif // _COMMAND_H_