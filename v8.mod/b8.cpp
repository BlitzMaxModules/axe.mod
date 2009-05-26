// b8.cpp
// blitzmax V8 stub
// by simon armstrong
// nitrologic@gmail.com

#include <v8.h>

using namespace v8;

extern "C" int v8main(char* script);

int v8main(char *js) {
 
// Create a new context.
  static Persistent<Context> context = Context::New();

// Create a stack-allocated handle scope.
  HandleScope handle_scope;
  
  // Enter the created context for compiling and
  // running the hello world script. 
  Context::Scope context_scope(context);

  // Create a string containing the JavaScript source code.
  Handle<String> source = String::New( js );  //"'Hello' + ', World!'");

  // Compile the source code.
  Handle<Script> script = Script::Compile(source);
  
  // Run the script to get the result.
  Handle<Value> result = script->Run();
  
  // Dispose the persistent context.
  context.Dispose();

  // Convert the result to an ASCII string and print it.

  String::AsciiValue ascii(result);

  if(*ascii){
    printf("%s\n", *ascii);
  }
  
  return 0;
}

