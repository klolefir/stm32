#include "fsm.h"

void decode(const req_buff_t *req_buff_st, dec_buff_t *dec_buff_st);
handle_st_t handle(const dec_buff_t *dec_buff_st, ans_buff_t *ans_buff_st);
void respond(ans_buff_t *ans_buff_st);
void purge(req_buff_t *req_buff_st);
void reset();
void process();
void fsm_process()
{

}
