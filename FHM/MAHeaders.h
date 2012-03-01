#define RES_IMAGE 1
#define RES_SELECTED_EDITBOX 2
#define RES_UNSELECTED_EDITBOX 3
#define RES_UNSELECTED_BUTTON 4
#define RES_TEXT_BOX 5
#define RES_BACKGROUND 6
#define RES_SELECTED_LIST 7
#define RES_UNSELECTED_LIST 8
#define RES_SELECTED_ALBUM 9
#define RES_UNSELECTED_ALBUM 10
#define RESP_DATA 11
#define RESP_IMAGE 12
#define RES_TEMPTHUMB 13
#define RES_TEMP 14
#define RES_LOADINGTHUMB 15
#define RES_MISSINGTHUMB 16
#define RES_CHANGE_STAR 17
#define RES_EMPTY_FLIP 18
#define RES_BOX_SELECTED 19
#define RES_BOX_UNSELECTED 20
#define RES_LOADING1 21
#define RES_LOADING2 22
#define RES_LOADING3 23
#define RES_LOADING4 24
#define RES_LOADING5 25
#define RES_LOADING6 26
#define RES_LOADING7 27
#define RES_LOADING8 28
#define RES_LOADING9 29
#define RES_LOADING10 30
#define RES_LOADING11 31
#define RES_LOADING12 32
#define RES_LOADING13 33
#define RES_LOADING14 34
#define RES_LOADING15 35
#define RES_LOADING16 36
#define RES_LOADING17 37
#define RES_LOADING18 38
#define RES_LOADING_FLIP1 39
#define RES_LOADING_FLIP2 40
#define RES_LOADING_FLIP3 41
#define RES_LOADING_FLIP4 42
#define RES_LOADING_FLIP5 43
#define RES_LOADING_FLIP6 44
#define RES_LOADING_FLIP7 45
#define RES_LOADING_FLIP8 46
#define RES_LOADING_FLIP9 47
#define RES_LOADING_FLIP10 48
#define RES_LOADING_FLIP11 49
#define RES_LOADING_FLIP12 50
#define RES_LOADING_FLIP13 51
#define RES_LOADING_FLIP14 52
#define RES_LOADING_FLIP15 53
#define RES_LOADING_FLIP16 54
#define RES_LOADING_FLIP17 55
#define RES_LOADING_FLIP18 56
#define RES_LEFT_ARROW 57
#define RES_RIGHT_ARROW 58
#define RES_ALBUM 59
#define RES_ALBUM_THUMB 60
#define RES_AUCTIONS 61
#define RES_AUCTIONS_THUMB 62
#define RES_CREDITS 63
#define RES_CREDITS_THUMB 64
#define RES_DECKS 65
#define RES_DECKS_THUMB 66
#define RES_FRIENDRANKS 67
#define RES_FRIENDRANKS_THUMB 68
#define RES_FRIENDS 69
#define RES_FRIENDS_THUMB 70
#define RES_INVITE 71
#define RES_INVITE_THUMB 72
#define RES_LOGOUT 73
#define RES_LOGOUT_THUMB 74
#define RES_NOTIFICATIONS 75
#define RES_NOTIFICATIONS_THUMB 76
#define RES_PLAY 77
#define RES_PLAY_THUMB 78
#define RES_PROFILE 79
#define RES_PROFILE_THUMB 80
#define RES_RANKINGS 81
#define RES_RANKINGS_THUMB 82
#define RES_REDEEM 83
#define RES_REDEEM_THUMB 84
#define RES_SHOP 85
#define RES_SHOP_THUMB 86

#define RES_DESELECTED_LIST_DOT 87
#define RES_SELECTED_LIST_DOT 88

#define RES_ICONS_BACK 89
#define RES_SELECT_ICON 90
#define RES_UNSELECT_ICON 91

#define RES_HEAD 92

/* Custom Variables per App */

#define RES_FONT_RED 93
#define RES_FONT_WHITE 94

#define THUMB_HEIGHT 74
#define ITEM_HEIGHT 48

#define URL "http://www.mytcg.net/_phone/topcar/"
#define URLSIZE 35

//we also need to define the x and y coordinates at which to split the images up
#define EDITBOX_X_LEFT 16
#define EDITBOX_X_RIGHT 32
#define EDITBOX_Y_TOP 22
#define EDITBOX_Y_BOTTOM 26

#define BUTTON_X_LEFT 16
#define BUTTON_X_RIGHT 80
#define BUTTON_Y_TOP 16
#define BUTTON_Y_BOTTOM 80

#define BACKGROUND_X_LEFT 1
#define BACKGROUND_X_RIGHT 464
#define BACKGROUND_Y_TOP 1
#define BACKGROUND_Y_BOTTOM 399

#define HEAD_X_LEFT 1
#define HEAD_X_RIGHT 211
#define HEAD_Y_TOP 1
#define HEAD_Y_BOTTOM 43

#define LIST_X_LEFT 21
#define LIST_X_RIGHT 171
#define LIST_Y_TOP 0
#define LIST_Y_BOTTOM 40

#define LISTNOARROWS_X_LEFT 16
#define LISTNOARROWS_X_RIGHT 77
#define LISTNOARROWS_Y_TOP 16
#define LISTNOARROWS_Y_BOTTOM 32

#define ALBUM_X_LEFT 4
#define ALBUM_X_RIGHT 160
#define ALBUM_Y_TOP 0
#define ALBUM_Y_BOTTOM 67

#define TEXT_X_LEFT 16
#define TEXT_X_RIGHT 32
#define TEXT_Y_TOP 16
#define TEXT_Y_BOTTOM 32

//the default heights and widths for various widgets
#define DEFAULT_LABEL_HEIGHT 48
#define ALBUM_ITEM_HEIGHT 74
#define THUMBNAIL_HEIGHT 64
#define MIN_SOFTKEY_BAR_HEIGHT 42
#define DEFAULT_SMALL_LABEL_HEIGHT 24
#define DEFAULT_DETAIL_HEADER_HEIGHT 36
#define DEFAULT_DETAILS_ITEM_HEIGHT 80
#define PADDING 5
#define ICON_HEIGHT 69
#define DOT_HEIGHT 16
#define ARROW_WIDTH 29
#define MIN_MENU_ICON_WIDTH 50

//renumber the menu options if one is removed
//numbers must always run sequential for index
#define OP_ALBUMS 0
#define OP_SHOP 3
#define OP_AUCTIONS 4
#define OP_NOTIFICATIONS 5
#define OP_CREDITS 6
#define OP_PROFILE 7
#define OP_FRIENDS 10
#define OP_INVITEFRIENDS 11
#define OP_REDEEM 12
#define OP_LOGOUT 13

#include <MAUtil/String.h>

using namespace MAUtil;

#ifndef _MAHeaders_H_
#define _MAHeaders_H_

static String FILE_PREFIX = "topcar_";

#endif