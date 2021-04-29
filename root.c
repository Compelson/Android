#include <unistd.h>


int main(int argc, char *argv[]) {
if(geteuid() != 0)
{
 printf("Run as root");
 exit 1;
}
exit 0;
}