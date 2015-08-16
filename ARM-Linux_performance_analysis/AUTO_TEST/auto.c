#include <stdlib.h>                                                           
#include <stdio.h>
#include <unistd.h>

char action[10];
int switch_int = 0;

void exit_program()
{
        printf("EXIT!\n");
        exit(1);
}
void get_action()
{
    do {        
	
	    printf("Welcome to AUTO-TEST\n");
        printf("\t<1> for Context Switch Latency;\n");
	    printf("\t<2> for UNIX Interprocess Communication Latency;\n");
        printf("\t<3> for Memory Read Latency;\n");
	    printf("\t<4> for Test the Line Size Relationship;\n");
	    printf("\t<5> for Test Syscall Latency;\n");
        printf("\t<61> for RR scheduling using CPU-bound program with 90 90 priority;\n");
        printf("\t<62> for RR scheduling using CPU-bound program with 90 45 priority;\n");
        printf("\t<63> for FIFO scheduling using CPU-bound program with 90 90 priority;\n");
        printf("\t<64> for FIFO scheduling using CPU-bound program with 90 45 priority;\n");
        printf("\t<65> for CFS scheduling using CPU-bound program;\n");
        printf("\t<71> for RR scheduling using IO-bound program with 90 90 priority;\n");
        printf("\t<72> for RR scheduling using IO-bound program with 90 45 priority;\n");
        printf("\t<73> for FIFO scheduling using IO-bound program with 90 90 priority;\n");
        printf("\t<74> for FIFO scheduling using IO-bound program with 90 45 priority;\n");
        printf("\t<75> for CFS scheduling using IO-bound program;\n");
        printf("\t<88> for EXIT this program!\n");
        printf("Please select what you want to do ? ");


        
                scanf("%s", action);
                switch_int = atoi(action);
                switch (switch_int) {
                case 1:
                        chdir("./lmbench-next/AUTO_TEST/");
			system("./LAT_CTX.sh");
			chdir("../../");
                        break;
                case 2:
			chdir("./lmbench-next/AUTO_TEST/");
			system("./LAT_UNIX.sh");
			chdir("../../");
                        break;
                case 3:
			chdir("./lmbench-next/AUTO_TEST/");
			system("./LAT_MEM_RD.sh");
			chdir("../../");
                        break;
                case 4:
			chdir("./line_size/");
			system("./LAT_LINE.sh");
			chdir("../");
                        break;
		        case 5: 
			chdir("./lmbench-next/AUTO_TEST/");
			system("./LAT_SYSCALL.sh");
			chdir("../");
			            break;
                case 61:
            chdir("./CPU-bound/");
            system("./cpu_rr_90_90.sh");
            chdir("../");
                        break;
                case 62:
            chdir("./CPU-bound/");
            system("./cpu_rr_90_45.sh");
            chdir("../");
                        break; 
                case 63:
            chdir("./CPU-bound/");
            system("./cpu_fifo_90_90.sh");
            chdir("../");
                        break;
                case 64:
            chdir("./CPU-bound/");
            system("./cpu_fifo_90_45.sh");
            chdir("../");
                        break;
                case 65:
            chdir("./CPU-bound/");
            system("./cpu_cfs.sh");
            chdir("../");
                        break;
                case 71:
            chdir("./IO-bound/");
            system("sh io_rr_90_90.sh");
            chdir("../");
                        break;
                case 72:
            chdir("./IO-bound/");
            system("./io_rr_90_45.sh");
            chdir("../");
                        break; 
                case 73:
            chdir("./IO-bound/");
            system("./io_fifo_90_90.sh");
            chdir("../");
                        break;
                case 74:
            chdir("./IO-bound/");
            system("./io_fifo_90_45.sh");
            chdir("../");
                        break;
                case 75:
            chdir("./IO-bound/");
            system("./io_cfs.sh");
            chdir("../");
                        break;
                case 88:
                        printf("Bye-Bye\n");
                        exit_program();
                        break;
                default:
                        printf("Try again!\n");
                        break;
                }
        } while (switch_int != 88);

}

int main(void)
{
        get_action();
}

