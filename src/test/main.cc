#include <iostream>
#include <limits>
#include <stdio.h>

using namespace std;

#define DM_MINIMUM_ALIGNMENT_LENGTH (128 * 512)

#define DM_MINIMUM_ALIGNMENT_SIZE (128 * 512)

#define CSX_MIN(x, y)   (((x) <= (y)) ? (x) : (y))

void DM_align_request(
    unsigned long long src_offset, 
    unsigned long long dst_offset,
    unsigned long length,
    unsigned long* adj_length)
{
    *adj_length = length;

    /* See if this request is long enough to bother with aligning
     */
    if (length <= DM_MINIMUM_ALIGNMENT_LENGTH) {
        printf("DM: Skipping alignment src: 0x%llx dst: 0x%llx len: 0x%x\n",
                                  src_offset, dst_offset, length);
        return;
    }
    
    /* See if the starting offsets are aligned. If not, adjust the length so that we
     * only transfer the sectors that aren't aligned.
     */
    if ((src_offset & (DM_MINIMUM_ALIGNMENT_SIZE - 1)) != 0) {
        *adj_length = DM_MINIMUM_ALIGNMENT_SIZE - (src_offset & (DM_MINIMUM_ALIGNMENT_SIZE - 1));
    }
    
    if ((dst_offset & (DM_MINIMUM_ALIGNMENT_SIZE - 1)) != 0) {
        *adj_length = CSX_MIN(*adj_length, (DM_MINIMUM_ALIGNMENT_SIZE - (dst_offset & (DM_MINIMUM_ALIGNMENT_SIZE-1))));
    }
    
    if (length == *adj_length) {
        if (((src_offset + length) & (DM_MINIMUM_ALIGNMENT_SIZE - 1)) != 0) {
            *adj_length = length & ~(DM_MINIMUM_ALIGNMENT_SIZE - 1);
        }
        if (((dst_offset + *adj_length) & (DM_MINIMUM_ALIGNMENT_SIZE - 1)) != 0) {
            *adj_length = *adj_length & ~(DM_MINIMUM_ALIGNMENT_SIZE - 1);
        }
    }
    printf("DM: Aligned requested src: 0x%llx dst: 0x%llx orig len: 0x%x new len: 0x%x\n",
                             src_offset, dst_offset, length, *adj_length);
    
    return;
}


int main(int argc, char* argv[])
{
    unsigned long adj_length;
    unsigned long long src_offset = 0X32A4F3C00;
    unsigned long long dst_offset = 0X32A4F3C00;
    unsigned long length = 989 * 512;
    
    DM_align_request(src_offset, dst_offset, length, &adj_length);
    
    printf("0x%x ( %u )\n", adj_length / 512, adj_length / 512 );
   
}
