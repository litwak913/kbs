#ifndef __DDD_H__
#define __DDD_H__

#ifdef DDD_ACTIVE

struct ddd_global_status {
    int type;
    int sec;
    int favid;
    int bid;
    int mode;
    int pos;
    int filter;
    int recur;
};


#define GS_NONE 0 // �����Ķ�״̬
#define GS_ALL 1 // ���а����б����߷���������
#define GS_NEW 2 // �·���������
#define GS_FAV 3 // ���˶�����
#define GS_GROUP 4 // Ŀ¼����
#define GS_BOARD 5 // ����
#define GS_MAIL 6 // ����

#define DDD_GS_CURR (getSession()->gs_curr)
#define DDD_GS_NEW (getSession()->gs_new)


int ddd_gs_init(struct ddd_global_status* gs);
int ddd_entry();
int ddd_read_loop();
int ddd_header();
int ddd_read_all();
int ddd_read_unknown();

#endif /* DDD_ACTIVE */

#endif /* __DDD_H__ */