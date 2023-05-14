#include <linux/blkdev.h>
#include <linux/blk-mq.h>

//Old Example
//https://github.com/CodeImp/sblkdev/blob/master/device.c

static struct request_queue* deviceQueue = NULL;
static struct blk_mq_tag_set deviceTagSet;

static blk_status_t block_request(struct blk_mq_hw_ctx *hctx, const struct blk_mq_queue_data *bd)
{
    blk_status_t status = BLK_STS_OK;
    struct request *rq = bd->rq;
    blk_mq_start_request(rq);
    // Do_simple_request
    blk_mq_end_request(rq, status);

}

static struct blk_mq_ops my_queue_ops = {
   .queue_rq = block_request,
};

void init(void)
{
    //Set tag_set
    //Aloocate tags with blk_mq_alloc_tag_set()
    //Config queue blk_mq_init_queue()
    //Set queue in gendisk struct
}

void exit(void)
{
    blk_mq_destroy_queue(deviceQueue); //changed in new kernel
    blk_mq_free_tag_set(&deviceTagSet);
}