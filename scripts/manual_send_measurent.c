#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>


void die(char *s) {
    perror(s);
    exit(1);
}

int main() {
    struct sockaddr_in si_other;
    int s, slen = sizeof(si_other);
    char buf[] = "can_id_test"; 

   
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        die("socket");
    }

   
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(8888); // random port
    if (inet_aton("127.0.0.1", &si_other.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

  
    struct timespec start, end;
    long long total_elapsed_time = 0;
    int iterations = 20; 

    for (int i = 0; i < iterations; ++i) {
        clock_gettime(CLOCK_MONOTONIC, &start);

        if (sendto(s, buf, strlen(buf), 0, (struct sockaddr *) &si_other, slen) == -1) {
            die("sendto()");
        }
 
        clock_gettime(CLOCK_MONOTONIC, &end);

        long long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
        printf("Elapsed time: %lld ns\n", elapsed_ns);
        total_elapsed_time += elapsed_ns;
    }


    close(s);

    printf("Number of iterations: %d\n", iterations);
    printf("Total elapsed time: %lld ns\n", total_elapsed_time);
    printf("Average time per sendto() call: %.2f ns\n", (double)total_elapsed_time / iterations);

    return 0;
}


