#define everage_score                          "Everage score" 
#define getting_the_average_score        "Getting the average score"
typedef struct _SCORE
{
    char *name;
    int scr[SUB_NUM];
} SCORE;

static char * caption [] =
{
    "Name", "Chinese", "Math", "English"
};

static char *classes [] =
{
    "Grade 1", "Grade 2", "Grade 3"
};

static SCORE scores[] =
{
    {"Tom", {81, 96, 75}},
    {"Jack", {98, 62, 84}},
    {"Merry", {79, 88, 89}},
    {"Bob", {79, 88, 89}},
};

