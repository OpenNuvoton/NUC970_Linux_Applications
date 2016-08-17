/* DMA Proxy Test Application
 *
 * This application is intended to be used with the DMA Proxy device driver. It provides
 * an example application showing how to use the device driver to do user space DMA
 * operations.
 *
 */
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include "dma_proxy.h"

/* The following function uses the dma proxy device driver to perform DMA transfers
 * from user space.
 */
int main(int argc, char *argv[])
{
    struct nuc970_mem_dma_param dma_param;
    struct nuc970_mem_alloc mem_alloc[2];
    char *mmap_addr[2];
    int cmd;
    int fd, i;
    int dummy;
    pthread_t tid;

    printf("DMA proxy test\n");

    /* Step 1, open the DMA proxy device for the transmit and receive channels with
     * read/write permissions
     */

    fd = open("/dev/dma_proxy", O_RDWR);

    if (fd < 1) {
        printf("Unable to open DMA proxy device file");
        return -1;
    }

    mem_alloc[0].vir_addr=0;
    mem_alloc[0].phy_addr=0;
    mem_alloc[0].size=1024*1;
    cmd = NUC970_DMA_ALLOC;
    mmap_addr[0] = (char *) mmap(0, mem_alloc[0].size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(mmap_addr[0] == NULL) {
        printf("input file memory mapping failed\n");
        return -1;
    }

    ioctl(fd, (unsigned int)&cmd, &mem_alloc[0]);

    mem_alloc[1].vir_addr=0;
    mem_alloc[1].phy_addr=0;
    mem_alloc[1].size=1024*1;
    cmd = NUC970_DMA_ALLOC;
    mmap_addr[1] = (char *) mmap(0, mem_alloc[1].size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    if(mmap_addr[1] == NULL) {
        printf("input file memory mapping failed\n");
        return -1;
    }
    ioctl(fd, (unsigned int)&cmd, &mem_alloc[1]);

    for(i=0; i<mem_alloc[1].size; i++) {
        *((unsigned char *)(mmap_addr[0])+i)=0;
        *((unsigned char *)(mmap_addr[1])+i)=i;
    }

    dma_param.size=1024*1;
    dma_param.src_addr=(unsigned int)mem_alloc[1].phy_addr;
    dma_param.dst_addr=(unsigned int)mem_alloc[0].phy_addr;
    dma_param.cfg= 0 ;
    cmd = NCU970_DMA_TRANSFER;
    ioctl(fd, (unsigned int)&cmd, &dma_param);
    printf("Compare");
    for(i=0; i<mem_alloc[0].size; i++) {
        if(*((unsigned char *)(mmap_addr[0])+i)!=*((unsigned char *)(mmap_addr[1])+i)) {
            printf("[Compare Error]%d %d %d\n",i,
                   *((unsigned char *)(mmap_addr[0])+i),
                   *((unsigned char *)(mmap_addr[1])+i)
                  );
            return 0;
        }
        if(i%1000==0)
            printf(".");
    }
    printf("\nCompare Pass\n");
    /* Unmap the proxy channel interface memory and close the device files before leaving
     */
    munmap(mmap_addr[0], mem_alloc[0].size);
    munmap(mmap_addr[1], mem_alloc[1].size);
    close(fd);
    return 0;
}
