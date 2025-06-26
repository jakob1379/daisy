#include "util/run_cmd.h"

std::string quote_if_unquoted(const std::string& str) {
    if (str.size() < 2 || str.front() != '"' || str.back() != '"') {
        return '"' + str + '"';
    }
    return str;
}

#if defined(_WIN32) || defined(__CYGWIN__32__)
#include <windows.h>
#include <processthreadsapi.h>
std::string run_cmd(const std::string & exe, const std::string & args, const std::string & name) {
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);
  ZeroMemory( &pi, sizeof(pi) );
  CreateProcessA(exe.c_str(), const_cast<char *>(args.c_str()), NULL, NULL, false, 0, NULL, NULL, &si, &pi);
  WaitForSingleObject( pi.hProcess, INFINITE );
  CloseHandle( pi.hProcess );
  CloseHandle( pi.hThread );
  return name;

}
#else
#include <cstdlib>
std::string run_cmd(const std::string & exe, const std::string & args, const std::string & name) {
    std::system((quote_if_unquoted(exe) + " " + args).c_str());
    return name;
}
#endif
