/*
 * $Id$
 */
#include "bbslib.h"

int my_strncmp (
        const char * first,
        const char * last,
        size_t count
        )
{
        if (!count)
                return(0);

        while (--count && *first && *first == *last)
        {
                first++;
                last++;
        }

        return( *(unsigned char *)first - *(unsigned char *)last );
}

int main()
{
	FILE *fp;
	char buf[1024], title[80], board[80], dir[80], first_file[80];
	char brdencode[STRLEN];
	bcache_t *x1;
	struct fileheader x, x0;
	int sum=0, total=0;

 	init_all();
	strsncpy(board, getparm("board"), 32);
	strsncpy(title, getparm("title"), 42);
	x1=getbcache(board);
	if(x1==0)
		http_fatal("错误的讨论区");
	strcpy(board, x1->filename);
	if(!has_read_perm(currentuser, board))
		http_fatal("错误的讨论区");
	sprintf(dir, "boards/%s/.DIR", board);
	fp=fopen(dir, "r");
	if(fp==0)
		http_fatal("错误的讨论区目录");
	encode_url(brdencode, board, sizeof(brdencode));
	sprintf(buf, "bbsman?board=%s&mode=1", brdencode);
	printf("<center>%s -- 同主题查找 [讨论区: %s] [主题 '%s']<hr class=\"default\">\n", 
		BBSNAME, board, nohtml(title));
	printf("<table border=\"1\"><tr><td>编号</td><td>作者</td><td>日期</td><td>标题</td></tr>\n");
	while(1)
	{
		if(fread(&x, sizeof(x), 1, fp)==0)
			break;
		sum++;
		if(!my_strncmp(title, x.title, 40) || 
		  (!my_strncmp(title, x.title+4, 40) && !my_strncmp(x.title, "Re: ", 4)))
		{
			if(total==0)
				strcpy(first_file, x.filename);
			printf("<tr><td>%d</td>", sum);
			printf("<td>%s</td>", userid_str(x.owner));
			if(!(x.accessed[0]&(FILE_MARKED|FILE_DIGEST)))
			{
				char buf2[STRLEN];
				snprintf(buf2, sizeof(buf2), "&box%s=on", x.filename);
				if(strlen(buf)<500)
					strcat(buf, buf2);
			}
			printf("<td>%6.6s</td>", wwwCTime(atoi(x.filename+2))+4);
			printf("<td><a href=\"bbscon?board=%s&file=%s&num=%d\">%s</a></td></tr>\n", 
				brdencode, x.filename, sum-1, void1(x.title));
			total++;
		}
	}
	fclose(fp);
	printf("</table><hr>\n共找到 %d 篇 \n", total);
	printf("<a href=\"bbsdoc?board=%s\">本讨论区</a> ", brdencode);
	if(total>0)
	{
		printf("<a href=\"bbstcon?board=%s&file=%s\">本主题全部展开</a> ",
				brdencode, first_file);
		if(has_BM_perm(currentuser, board)) 
			printf("<a onclick=\"return confirm('确定同主题全部删除?')\" href=\"%s\">同主题删除</a>", buf);
	}
	http_quit();
}
