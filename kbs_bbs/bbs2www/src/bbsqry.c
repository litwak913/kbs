/*
 * $Id$
 */
#include "bbslib.h"

char genbuf[ 1024 ];

int
show_user_plan(userid)
char userid[IDLEN];
{
    int i;
    char pfile[STRLEN],pbuf[256];
    FILE *pf;

    sethomefile(pfile,userid,"plans");
    if ((pf = fopen(pfile, "r")) == NULL)
    {
        hprintf("[36m没有个人说明档[m\n");
        return false;
    }
    else
    {
        hprintf("[36m个人说明档如下：[m\n");
        for (i=1; i<=MAXQUERYLINES; i++)
        {
            if (fgets(pbuf, sizeof(pbuf), pf))
                hprintf("%s", pbuf);
            else break;
        }
        fclose(pf);
        return true;
    }
}

int t_printstatus(struct user_info* uentp,int* arg,int pos)
{
    if(uentp->invisible==1)
    {
        if(!HAS_PERM(currentuser,PERM_SEECLOAK))
	        return COUNT;
    }
    (*arg)++;
    if(*arg==1)
        strcpy(genbuf,"目前在站上，状态如下：\n");
    if (uentp->invisible)
        strcat(genbuf,"[32m隐身中   [m");
    else {
    	char buf[80];
    	sprintf(buf,"[1m%s[m ", modestring(uentp->mode,
                                     uentp->destuid, 0,/* 1->0 不显示聊天对象等 modified by dong 1996.10.26 */
                                     (uentp->in_chat ? uentp->chatid : NULL)));
        strcat(genbuf,buf);
    }
    if((*arg)%8==0)
           strcat(genbuf,"\n");
    UNUSED_ARG(pos);
    return COUNT;
}

void display_user(char *userid)
{
    char        uident[STRLEN], *newline ;
    int         tuid=0;
    int         exp,perf;
    struct user_info uin;
    char qry_mail_dir[STRLEN];
    char planid[IDLEN+2];
    char permstr[10];
    char exittime[40];
    time_t exit_time,temp;
    int logincount,seecount;
    struct userec* lookupuser;
	uinfo_t *ui;
	uinfo_t guestui;

	printf("</center><pre>\n");
    strcpy(uident, strtok(userid, " "));
    if(!(tuid = getuser(uident,&lookupuser)))
	{
		printf("用户 [%s] 不存在.", userid);
		http_quit();
    }
	ui = getcurruinfo();
	if (ui == NULL)
	{
		ui = &guestui;
		ui->in_chat = 0;
	}
	ui->destuid = tuid;

    setmailfile(qry_mail_dir, lookupuser->userid, DOT_DIR);

    exp=countexp(lookupuser);
    perf=countperf(lookupuser);
    /*---	modified by period	2000-11-02	hide posts/logins	---*/
    hprintf( "%s (%s) 共上站 %d 次，发表过 %d 篇文章",
                lookupuser->userid, lookupuser->username,
                lookupuser->numlogins,lookupuser->numposts);
    strcpy(planid,lookupuser->userid);
    if( (newline = strchr(genbuf, '\n')) != NULL )
        *newline = '\0';
    seecount=0;
	logincount=apply_utmp(t_printstatus,10,lookupuser->userid,&seecount);
    /* 获得离线时间 Luzi 1998/10/23 */
    exit_time = get_exit_time(lookupuser->userid,exittime);
    if( (newline = strchr(exittime, '\n')) != NULL )
        *newline = '\0';

    if (exit_time <= lookupuser->lastlogin)
	{
    	if (logincount!=seecount)
	    {
    	    temp=lookupuser->lastlogin+((lookupuser->numlogins+lookupuser->numposts)%100)+60;
        	strcpy(exittime,ctime(&temp));/*Haohmaru.98.12.04.让隐身用户看上去离线时间比上线时间晚60到160秒钟*/
	        if( (newline = strchr(exittime, '\n')) != NULL )
    	        *newline = '\0';
	    }
		else
    	    strcpy(exittime,"因在线上或非常断线不详");
	}
    hprintf( "\n上次在  [%s] 从 [%s] 到本站一游。\n离线时间[%s] ",
			wwwCTime(lookupuser->lastlogin),
            ((lookupuser->lasthost[0] == '\0') ? 
			 	"(不详)" : lookupuser->lasthost),
            exittime);
    uleveltochar(&permstr,lookupuser);
    hprintf("信箱：[[5m%2s[m] 生命力：[%d] 身份: [%s]%s\n",
           (check_query_mail(qry_mail_dir)==1)? "信":"  ",
           compute_user_value(lookupuser),
           permstr,(lookupuser->userlevel & PERM_SUICIDE)?" (自杀中)":"。");

	if ((genbuf[0])&&seecount) {
		hprintf("%s",genbuf);
		printf("\n");
	}
    show_user_plan(planid);
	printf("</pre>");
	printf("<br><br><a href=\"bbspstmail?userid=%s&title=没主题\">[写信问候]</a> ", lookupuser->userid);
	printf("<a href=\"bbssendmsg?destid=%s\">[发送讯息]</a> ",
			lookupuser->userid);
	printf("<a href=\"bbsfadd?userid=%s\">[加入好友]</a> ", lookupuser->userid);
	printf("<a href=\"bbsfdel?userid=%s\">[删除好友]</a>", lookupuser->userid);
	printf("<hr>");
	printf("</center>\n");

    ui->destuid = 0;
}

int main()
{
	char userid[14];

	init_all();
	strsncpy(userid, getparm("userid"), 13);
	printf("<center>");
	printf("%s -- 查询网友<hr color=green>\n", BBSNAME);
	if(userid[0]==0) {
		printf("<form action=bbsqry>\n");
		printf("请输入用户名: <input name=userid maxlength=12 size=12>\n");
		printf("<input type=submit value=查询用户>\n");
		printf("</form><hr>\n");
		http_quit();
	}
	display_user(userid);
	http_quit();
}

