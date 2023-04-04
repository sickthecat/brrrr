#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {
    struct hostent *host;
    struct in_addr ipv4addr;
    char **p;

    if (argc != 2) {
        printf("Usage: %s ip_range_cidr\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *ip_range_cidr = argv[1];

    // Parse the CIDR notation to get the IP range
    char *ip = strtok(ip_range_cidr, "/");
    int netmask_bits = atoi(strtok(NULL, "/"));
    uint32_t ip_int = ntohl(inet_addr(ip));
    uint32_t netmask_int = ~(0xFFFFFFFF >> netmask_bits);

    // Calculate the start and end IP addresses based on the CIDR notation
    uint32_t start_int = ip_int & netmask_int;
    uint32_t end_int = start_int | ~netmask_int;

    // Open the output file
    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        printf("Failed to open output file\n");
        exit(EXIT_FAILURE);
    }

    // Iterate over IP addresses in the range and write the output to the file and print to stdout
    for (uint32_t i = start_int; i <= end_int; i++) {
        ipv4addr.s_addr = htonl(i);
        host = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

        // Print each hostname associated with the IP address to the file and to stdout
        if (host != NULL) {
            fprintf(output_file, "%s:\n", inet_ntoa(ipv4addr));
            printf("%s:\n", inet_ntoa(ipv4addr));
            for (p = host->h_aliases; *p != NULL; p++) {
                fprintf(output_file, "  %s\n", *p);
                printf("  %s\n", *p);
            }
            fprintf(output_file, "  %s\n", host->h_name);
            printf("  %s\n", host->h_name);
        }
    }

    // Close the output file
    fclose(output_file);

    return 0;
}
