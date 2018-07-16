#include"ir.h"
#include"runtime.h"
#include"parser.tab.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


struct horde_node *horde = NULL;

//selects all ALIVE data, adds them to the horde
void run_init_runtime(struct ir_node *i)
{
	int liveones = 0;

	while(i != NULL)
	{
		if(i->state == ALIVE)
		{
			struct horde_node *h=(struct horde_node *)malloc(sizeof(struct horde_node));
			h->orc = i;
			h->next = horde;
			h->previous = NULL;
			horde = h;
			liveones++;
		}
		i = i->next;
	}
	printf("Horde initialized: found %d live ones.\n",liveones);
}

//Adds a single IR node to the horde
void run_add_to_horde(struct ir_node *i)
{
	struct horde_node *h=(struct horde_node *)malloc(sizeof(struct horde_node));
	h->orc = i;
	h->next = horde;
	h->previous = NULL;
	horde = h;
}

//propagates datum life to other data, kills datum, removes from horde, adds alive ones to horde
void run_prop_life(struct horde_node *h)
{
	struct ir_node *i = h->orc;

	//TODO: perform operation
	run_do_op(h);

	//IF we're still alive after performing operation
	//(maybe we killed ourselves)
	if(i->state == ALIVE)
	{
		//propagate life and value
		while(i->depend_list != NULL)
		{
			if(i->depend_list->side == D_LEFT)
			{
				i->depend_list->d_node->l_value = i->l_value;
				i->depend_list->d_node->state--;
				if(i->depend_list->d_node->state == ALIVE)
					run_add_to_horde(i->depend_list->d_node);
			}
			else
			{
				i->depend_list->d_node->r_value = i->l_value;
				i->depend_list->d_node->state--;
				if(i->depend_list->d_node->state == ALIVE)
					run_add_to_horde(i->depend_list->d_node);
			}
			i->depend_list = i->depend_list->next;
		}
	}
	//Kill datum, if not already dead
	i->state = DEAD;
	//Remove from horde
	//If we're not first datum (horde pointer)
	if(h->previous != NULL)
	{
		h->previous->next = h->next;
		//If we're not last datum
		if(h->next != NULL)
			h->next->previous = h->previous;
	}
	else
	{
		if(h->next != NULL)
			h->next->previous = h->previous;
		horde = h->next;
	}
	free(h);
}

//Performs datum's operation
void run_do_op(struct horde_node *h)
{
	switch(h->orc->operation)
	{
	case ADD:
	{
		h->orc->l_value = h->orc->l_value + h->orc->r_value;
	} break;
	case SUB:
	{
		h->orc->l_value = h->orc->l_value - h->orc->r_value;
	} break;
	case MUL:
	{
		h->orc->l_value = h->orc->l_value * h->orc->r_value;
	} break;
	case DIV:
	{
		h->orc->l_value = h->orc->l_value / h->orc->r_value;
	} break;
	case MOD:
	{
		h->orc->l_value = h->orc->l_value % h->orc->r_value;
	} break;
	case EQUALS:
	{
		h->orc->l_value = (h->orc->l_value == h->orc->r_value) ? 1: 0;
	} break;
	case NOTEQUAL:
	{
		h->orc->l_value = (h->orc->l_value != h->orc->r_value) ? 1: 0;
	} break;
	case GREATER:
	{
		h->orc->l_value = (h->orc->l_value > h->orc->r_value) ? 1: 0;
	} break;
	case LESS:
	{
		h->orc->l_value = (h->orc->l_value < h->orc->r_value) ? 1: 0;
	} break;
	case GREATEREQ:
	{
		h->orc->l_value = (h->orc->l_value >= h->orc->r_value) ? 1: 0;
	} break;
	case LESSEQ:
	{
		h->orc->l_value = (h->orc->l_value <= h->orc->r_value) ? 1: 0;
	} break;
	case IF:
	{
		if(h->orc->l_value)
			h->orc->l_value = h->orc->r_value;
		else
			h->orc->state = DEAD;
	} break;
	case LIFE:
	{
		//We dont know who gave us life; as default value is 0, should be safe to add for now
		//TODO: modify once we support more than integers
		h->orc->l_value += h->orc->r_value;
	} break;
	case ZOMBIE:
	{
	} break;
	case SOURCE:
	{
		//Do nothing, already l_value
	} break;
	default: printf("Error: unknown operation in datum %s.\n",h->orc->name); break;
	}
}



//goes through the horde list, evaluates all alive ones
//updates life and death and evaluates
int run_horde()
{
	struct horde_node *h;
	char c;

	while(horde != NULL)
	{
		h = horde;	
		run_prop_life(h);
		h = h->next;

		if(h == NULL)
			h = horde;

		scanf("%c",&c);
	}
}






