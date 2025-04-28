/* See LICENSE file for copyright and license details. */

/* appearance */

#if !PERTAG_PATCH
static int enablegaps = 0;
#endif
static const unsigned int borderpx  = 6;        /* border pixel of windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 0;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 0;       /* vert inner gap between windows */
static const unsigned int gappoh    = 0;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 0;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "CaskaydiaCove Nerd Font:size=20" };
static const char dmenufont[]       = "CaskaydiaCoveNerdFont:size=20";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#2f2f55";
static const char *colors[][3]      = {
    /*                      fg         bg         border   */
    [SchemeNorm]     = {col_gray1, col_cyan,  "#000000"},
    [SchemeSel]      = {col_gray4, col_cyan,  "#606080"},
    [SchemeStatus]   = {col_gray4, col_cyan,  "#000000"}, // Statusbar right {text,background,not used but cannot be empty}
    [SchemeTagsSel]  = {col_gray1, "#606080", "#000000"}, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm] = {col_gray3, col_cyan,  "#000000"}, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]  = {col_gray1, "#606080", "#000000"}, // infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm] = {col_gray3, col_cyan,  "#000000"}, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "󰈹"};

/* Lockfile */
static char lockfile[] = "/tmp/dwm.lock";

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class,       instance,  title,           tags mask,  isfloating,  x,    y,    w,   h,   isterminal,  noswallow,  monitor */
    {"Gimp",		NULL,      NULL,            0,          1,           -1,   -1,   -1,  -1,  0,           0,          -1},
    {"firefox",		NULL,      NULL,            1 << 8,     0,           -1,   -1,   -1,  -1,  0,           -1,         1},
    {"Alacritty",	NULL,      NULL,            0,          0,           -1,   -1,   -1,  -1,  1,           0,          -1},
    {"filemanager",	NULL,      NULL,            0,          1,           .25,  .25,  .5,  .5,  1,           0,          -1},
    {"term",		NULL,      NULL,            0,          1,           .25,  .25,  .5,  .5,  1,           0,          -1},
    {NULL,			NULL,      "Event Tester",  0,          0,           -1,   -1,   -1,  -1,  0,           1,          -1},       /* xev */
};

/* layout(s) */
static const float mfact        = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster        = 1;    /* number of clients in master area */
static const int resizehints    = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const scratchpad filemanager      = {.class = "filemanager", .v = (char *[]) {
    "alacritty", "--class", "filemanager", "-e", "lf", NULL
}
                                           };
static const scratchpad term    = {.class = "term", .v = (char *[]) {
    "alacritty", "--class", "term", NULL
}
                                  };

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */

static const Layout layouts[] = {
    /* symbol   arrange function */
    {"[]=",     tile}, /* first entry is default */
    {"[M]",     monocle},
    {"|M|",     centeredmaster},
    {"TTT",     bstack},
    {"H[]",     deck},
    {"><>",     NULL},    /* no layout function means floating behavior */
    {NULL,      NULL},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                           \
            {MODKEY,                        KEY,  view,        {.ui = 1 << TAG}},   \
            {MODKEY|ControlMask,            KEY,  toggleview,  {.ui = 1 << TAG}},   \
            {MODKEY|ShiftMask,              KEY,  tag,         {.ui = 1 << TAG}},   \
            {MODKEY|ControlMask|ShiftMask,  KEY,  toggletag,   {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) {.v = (const char *[]) {"/bin/sh", "-c", cmd, NULL}}

/* commands */
static const char *roficmd[]      = {"rofi", "-show", "drun", NULL};
static const char *rofirun[]      = {"rofi", "-show", "run", NULL};
static const char *rofiwindow[]      = {"rofi", "-show", "window", NULL};
static const char *termcmd[]       = {"alacritty", NULL};
static const char *lockScreen[]    = {"loginctl", "lock-session", NULL};
static const char *nextWallpaper[] = {"setbackground", "next", NULL};
static const char *prevWallpaper[] = {"setbackground", "prev", NULL};
static const Key keys[] = {
    /*  modifier,                   key,                       function,        argument */
    {MODKEY,                        XK_p,                      spawn,           {.v = roficmd}},
    {MODKEY,                        XK_r,                      spawn,           {.v = rofirun}},
    {MODKEY,                        XK_Escape,                 spawn,           SHCMD("shutdown now")},
    {MODKEY,                        XK_w,                      spawn,           {.v = rofiwindow}},
    {MODKEY|ShiftMask,              XK_Return,                 spawn,           SHCMD("cd $(xcwd); alacritty;")},
    {MODKEY,                        XK_b,                      togglebar,       {.i = -1}},                                                                                    // -1 means invert current value
    {MODKEY,                        XK_j,                      focusstack,      {.i = +1}},
    {MODKEY,                        XK_k,                      focusstack,      {.i = -1}},
    {MODKEY,                        XK_i,                      incnmaster,      {.i = +1}},
    {MODKEY,                        XK_d,                      incnmaster,      {.i = -1}},
    {MODKEY,                        XK_l,                      spawn,           {.v = lockScreen}},
    {MODKEY,                        XK_equal,                  setmfact,        {.f = +0.05}},
    {MODKEY,                        XK_minus,                  setmfact,        {.f = -0.05}},
    {MODKEY|ShiftMask,              XK_h,                      setcfact,        {.f = +0.25}},
    {MODKEY|ShiftMask,              XK_l,                      setcfact,        {.f = -0.25}},
    {MODKEY|ShiftMask,              XK_o,                      setcfact,        {.f =  0.00}},
    {MODKEY|ControlMask,            XK_equal,                  spawn,           {.v = nextWallpaper}},
    {MODKEY|ControlMask,            XK_minus,                  spawn,           {.v = prevWallpaper}},
    {MODKEY|ControlMask,            XK_u,                      incrgaps,        {.i = +1}},
    {MODKEY|ControlMask|ShiftMask,  XK_u,                      incrgaps,        {.i = -1}},
    {MODKEY|ControlMask,            XK_i,                      incrigaps,       {.i = +1}},
    {MODKEY|ControlMask|ShiftMask,  XK_i,                      incrigaps,       {.i = -1}},
    {MODKEY|ControlMask,            XK_o,                      incrogaps,       {.i = +1}},
    {MODKEY|ControlMask|ShiftMask,  XK_o,                      incrogaps,       {.i = -1}},
    {MODKEY|Mod1Mask,               XK_6,                      incrihgaps,      {.i = +1}},
    {MODKEY|Mod1Mask|ShiftMask,     XK_6,                      incrihgaps,      {.i = -1}},
    {MODKEY|Mod1Mask,               XK_7,                      incrivgaps,      {.i = +1}},
    {MODKEY|Mod1Mask|ShiftMask,     XK_7,                      incrivgaps,      {.i = -1}},
    {MODKEY|Mod1Mask,               XK_8,                      incrohgaps,      {.i = +1}},
    {MODKEY|Mod1Mask|ShiftMask,     XK_8,                      incrohgaps,      {.i = -1}},
    {MODKEY|Mod1Mask,               XK_9,                      incrovgaps,      {.i = +1}},
    {MODKEY|Mod1Mask|ShiftMask,     XK_9,                      incrovgaps,      {.i = -1}},
    {MODKEY|Mod1Mask,               XK_0,                      togglegaps,      {0}},
    {MODKEY|Mod1Mask|ShiftMask,     XK_0,                      defaultgaps,     {0}},
    {MODKEY,                        XK_Return,                 zoom,            {0}},
    {MODKEY|ControlMask,            XK_comma,                  cyclelayout,     {.i = -1}},
    {MODKEY|ControlMask,            XK_period,                 cyclelayout,     {.i = +1}},
    {MODKEY,                        XK_Tab,                    view,            {0}},
    {MODKEY,                        XK_y,                      togglescratch,   {.v = &filemanager}},
    {MODKEY,                        XK_u,                      togglescratch,   {.v = &term}},
    {MODKEY,                        XK_c,                      killclient,      {0}},
    {MODKEY|ShiftMask,              XK_q,                      quit,            {0}},
    {MODKEY,                        XK_space,                  setlayout,       {0}},
    {MODKEY,                        XK_f,                      togglefullscr,   {0}},
    {MODKEY|ShiftMask,              XK_space,                  togglefloating,  {0}},
    {MODKEY,                        XK_0,                      view,            {.ui = ~0}},
    {MODKEY|ShiftMask,              XK_0,                      tag,             {.ui = ~0}},
    {MODKEY,                        XK_comma,                  focusmon,        {.i = -1}},
    {MODKEY,                        XK_period,                 focusmon,        {.i = +1}},
    {MODKEY|ShiftMask,              XK_comma,                  tagmon,          {.i = -1}},
    {MODKEY|ShiftMask,              XK_period,                 tagmon,          {.i = +1}},
    {MODKEY,                        XK_Right,                  viewnext,        {0}},
    {MODKEY,                        XK_Left,                   viewprev,        {0}},
    {MODKEY|ShiftMask,              XK_Right,                  tagtonext,       {0}},
    {MODKEY|ShiftMask,              XK_Left,                   tagtoprev,       {0}},
    {0,                             XF86XK_MonBrightnessDown,  spawn,           SHCMD("brightnessctl set 5%-; pkill -36 dwmblocks")},
    {0,                             XF86XK_MonBrightnessUp,    spawn,           SHCMD("brightnessctl set 5%+; pkill -36 dwmblocks")},
    {0,                             XF86XK_AudioMute,          spawn,           SHCMD("amixer -D pulse set Master toggle; pkill -35 dwmblocks")},
    {0,                             XF86XK_AudioLowerVolume,   spawn,           SHCMD("amixer -D pulse set Master on; amixer -D pulse set Master 5%-; pkill -35 dwmblocks")},
    {0,                             XF86XK_AudioRaiseVolume,   spawn,           SHCMD("amixer -D pulse set Master on; amixer -D pulse set Master 5%+; pkill -35 dwmblocks")},
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click,        event mask,  button,   function,        argument */
    {ClkLtSymbol,    0,           Button1,  cyclelayout,       {.i = +1}},
    {ClkLtSymbol,    0,           Button3,  cyclelayout,       {.i = -1}},
    {ClkWinTitle,    0,           Button2,  zoom,            {0}},
    {ClkStatusText,  0,           Button2,  spawn,           {.v = termcmd}},
    {ClkClientWin,   MODKEY,      Button1,  movemouse,       {0}},
    {ClkClientWin,   MODKEY,      Button2,  togglefloating,  {0}},
    {ClkClientWin,   MODKEY,      Button3,  resizemouse,     {0}},
    {ClkTagBar,      0,           Button1,  view,            {0}},
    {ClkTagBar,      0,           Button3,  toggleview,      {0}},
    {ClkTagBar,      MODKEY,      Button1,  tag,             {0}},
    {ClkTagBar,      MODKEY,      Button3,  toggletag,       {0}},
};
