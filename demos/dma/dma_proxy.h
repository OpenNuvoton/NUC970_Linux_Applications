/* This header file is shared between the DMA Proxy test application and the DMA Proxy device driver. It defines the
 * shared interface to allow DMA transfers to be done from user space.
 *
 * Note: the buffer in the data structure should be 1st in the channel interface so that the buffer is cached aligned,
 * otherwise there may be issues when using cached memory. The issues were typically the 1st 32 bytes of the buffer
 * not working in the driver test.
 */

#define USE_DMA_ALLOC
#define NUC970_DMA_ALLOC        1
#define NCU970_DMA_TRANSFER 2

struct nuc970_mem_dma_param {
    int             size;
    unsigned int    src_addr;
    unsigned int    dst_addr;
    int             cfg;
};

struct nuc970_mem_alloc {
    int     size;
    unsigned int    vir_addr;
    unsigned int    phy_addr;

#ifdef USE_DMA_ALLOC
    unsigned int    kvir_addr;
#endif
};
