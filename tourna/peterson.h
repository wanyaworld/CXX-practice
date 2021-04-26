#include <stdio.h>

struct peterson
{
	int n;
	int bit;
	void* flags;
};

void init_lock(struct peterson* peterson, int n, void* flags);
int get_flags(int n);
int offset(int bit, int bit_cur, int id);
void _lock(struct peterson* peterson, int id);
void _unlock(struct peterson* peterson, int id);


struct flags
{
	int flag[2];
	int turn;
};


int get_flags(int n)
{
	int bit;
	for(bit = 0; (1 << bit) <= n; bit++);
	return sizeof(struct flags) * (offset(bit, 0, n - 1) + 1);
}

int offset(int bit, int bit_cur, int me)
{
	int offset{};
	int mask;
	for(bit--, mask = 1 << bit; bit > bit_cur; bit--, mask >>= 1, offset++)
		if(mask & me)
			offset += mask - 1;
	return offset;
}

void init_lock(struct peterson* peterson, int n, void* flags)
{
	for(peterson->bit = 0; (1 << peterson->bit) <= n; peterson->bit+=1);
	peterson->n = n;
	peterson->flags = flags;
}

void _lock(struct peterson* peterson, int id)
{
	int bit;
	int mask;
	struct flags* cur_flag;

	for(bit = 0, mask = 1; bit < peterson->bit; bit++, mask <<= 1)
	{
		int my_turn = mask & id;
		cur_flag = (struct flags*)peterson->flags;
		cur_flag += offset(peterson->bit, bit, id);
		cur_flag->flag[my_turn] = true;
		cur_flag->turn = !my_turn;
		__sync_synchronize();
		while(true == cur_flag->flag[!my_turn] && cur_flag->turn == !my_turn);
		__sync_synchronize();
	}
}

void _unlock(struct peterson* peterson, int id)
{
	int bit;
	int mask;
	struct flags* cur_flag;

	__sync_synchronize();
	for(bit = peterson->bit - 1, mask = 1 << (peterson->bit - 1); bit >= 0; bit--, mask >>= 1)
	{
		int my_turn = mask & id;
		cur_flag = (struct flags*)peterson->flags;
		cur_flag += offset(peterson->bit, bit, id);
		cur_flag->flag[my_turn] = false;
	}
	__sync_synchronize();
}
