#ifndef BLOADER_H_SENTRY
#define BLOADER_H_SENTRY

enum {
	bl_page_size = 256 //bytes
};

enum {
	bl_sig_pos 	= 0,
	bl_cmd_pos 	= bl_sig_pos,
	bl_data_pos = 1
};

#if 1
enum {
	bl_erase_cmd_len	= 1,
	bl_write_cmd_len 	= 1,
	bl_read_cmd_len	 	= 1
};
#endif

enum {
	bl_write_cmd,
	bl_read_cmd,
	bl_reset_cmd,
	bl_erase_cmd
};

#endif
