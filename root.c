#include <unistd.h>


int main(int argc, char *argv[]) {
if(geteuid() != 0)
{
 printf("Run this program as root\n");
 return 1;
}
 printf("Runnings as root\n");
return 0;
}