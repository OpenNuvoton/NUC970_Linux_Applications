#define everage_score "平均分"
#define getting_the_average_score "計算平均分"
typedef struct _SCORE
{
    char *name;
    int scr[SUB_NUM];
} SCORE;


static char * caption [] =
{
     "姓名", "語文", "數學", "英語"
};

static char *classes [] =
{
     "1班", "2班", "3班"
};
static SCORE scores[] =
{
     {"小明", {81, 96, 75}},
     {"小強", {98, 62, 84}},
     {"小亮", {79, 88, 89}},
     {"小力", {79, 88, 89}},
};
	
		
