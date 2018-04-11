#atl

Command line options parser example

```C++
#include "sys/cmndline.hpp"

using namespace atl::sys;

int main(int argc, char* argv[])
{
  /* Define command line argumnets, long option name are required */
	cmdline.options(argc, argv,
		command_line::option("aname", 1, 'a'),  /* long name[, option value required (0 - not require, 1 - require)[,short option name]] */
		command_line::option("bname", 0, 'b'),
		command_line::option("nname", 0, 'n'),
		command_line::option("cname", 1),
		command_line::option("dname", 0),
		command_line::option("ename", 1, 'e')
	);

	printf("[ %10s ] %8s val = %s\n", "aname", cmdline.is("aname") ? "SET" : "NOT SET", cmdline("aname", "empty"));
	printf("[ %10s ] %8s val = %s\n", "bname", cmdline.is("bname") ? "SET" : "NOT SET", cmdline("bname", "empty"));
	printf("[ %10s ] %8s val = %s\n", "cname", cmdline.is("cname") ? "SET" : "NOT SET", cmdline("cname", "empty"));
	printf("[ %10s ] %8s val = %s\n", "dname", cmdline.is("dname") ? "SET" : "NOT SET", cmdline("dname", "empty"));
	printf("[ %10s ] %8s val = %s\n", "ename", cmdline.is("ename") ? "SET" : "NOT SET", cmdline("ename", "empty"));
	printf("[ %10s ] %8s val = %s\n", "fname", cmdline.is("fname") ? "SET" : "NOT SET", cmdline("fname", "empty"));
	printf("[ %10s ] %8s val = %s\n", "nname", cmdline.is("nname") ? "SET" : "NOT SET", cmdline("nname", "empty"));
	
  return 0;
}

```
