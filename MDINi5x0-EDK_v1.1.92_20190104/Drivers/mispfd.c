// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------
#include	"misp100.h"

#if defined(__USE_FD_PROCESS__)
// ----------------------------------------------------------------------
// Struct/Union Types and define
// ----------------------------------------------------------------------
typedef struct {
	BYTE node_pos;
	BYTE node_cnt;
	LONG stag_thr;
} Lbp_Stag_t;

typedef struct {
	BYTE rect_no; //rect.no
	DWORD node_data[8];  //subset
	LONG leaf_val[2];
} Lbp_Node_t;

typedef struct {
	BYTE x;
	BYTE y;
	BYTE w;
	BYTE h;
} Lbp_Rect_t;

typedef struct {
	BYTE stag_cnt;
	BYTE node_cnt;
	BYTE rect_cnt;
	ROMDATA Lbp_Stag_t* p_lbp_stag;
	ROMDATA Lbp_Node_t* p_lbp_node;
	ROMDATA Lbp_Rect_t* p_lbp_rect;
} Lbp_Base_t;

// LBP FACADE(FACE) DBASE (24x24)
#define FACADE_HSIZE 24
#define FACADE_VSIZE 24
#define FACADE_STAG_CNT 10
#define FACADE_NODE_CNT 94
#define FACADE_RECT_CNT 93

static ROMDATA Lbp_Stag_t FACADE_STAG[FACADE_STAG_CNT]={
	{ 0,  4, -1638797},
	{ 4,  5, -1106104},
	{ 9,  8,  -863296},
	{17,  8, -1637656},
	{25,  9,  -854586},
	{34, 10,  -915776},
	{44, 13, -1010866},
	{57, 12,  -696145},
	{69, 12,  -974410},
	{81, 13, -1199453}
};

static ROMDATA Lbp_Node_t FACADE_NODE[FACADE_NODE_CNT]={
	{37, {0xd044f2d0, 0xf797c387, 0x404004d1, 0x5111db57, 0xf1039aaf, 0xdb1b8083, 0x7301faff, 0xffdfefff}, {-873662, 765478} },
	{23, {0xffdff7ff, 0x1985c08a, 0x5302819f, 0x0b072003, 0xf30bdbdb, 0x898bc282, 0x3307f2ff, 0x2f2ffb1f}, {-842100, 699471} },
	{62, {0xfc0dfac8, 0xd0554222, 0x8151d953, 0xdfdfc3d7, 0xc189e0c0, 0xd1b98080, 0xfbabfb8f, 0xfffffbff}, {-812119, 617821} },
	{34, {0xffffbbff, 0x7fff2eff, 0x3fbfaabe, 0x7fef8aaa, 0xffffafbb, 0xffffbfff, 0xfaf7a2aa, 0xfbf783ab}, {-652487, 739138} },
	{33, {0xffffbbff, 0xffffaaff, 0xffffa8bb, 0xffffa2bf, 0xffffbbbf, 0xfdd5aff7, 0xffffa8bb, 0xffffa2ab}, {-670470, 812051} },
	{76, {0xdc4cecc8, 0x4cd4e4af, 0xd00e916c, 0xddf9f9df, 0xf19cfad1, 0xd9c7ea89, 0xfdbbfaab, 0xfffffbff}, {-813540, 561299} },
	{10, {0xf7e6ff92, 0xfbccdd98, 0xdd085dd4, 0xfda0fdba, 0x7aa1fbbf, 0xffb1a7a2, 0xfbcbfabd, 0xffe5dbff}, {-688943, 603603} },
	{27, {0x5551f560, 0xddd1f180, 0xd707f0f9, 0xfff9b6e6, 0x4d04d151, 0xddd5ca83, 0xe11fffdb, 0xffafebeb}, {-777984, 547598} },
	{ 7, {0xf9dbd3ef, 0xffd959df, 0xd181410e, 0x4babf022, 0xfa0bbbdf, 0x7bbf80af, 0xfa87ea0b, 0x4167c44b}, {-697428, 519252} },
	{34, {0xffffffff, 0xffffaeff, 0xffbfabbb, 0xffffaaaa, 0xffffbfbf, 0xffffbfff, 0xffffaaaa, 0xffbfa2ab}, {-595007, 700599} },
	{ 4, {0xea42fee2, 0xe062e1ce, 0xee6813d7, 0xff7b841b, 0x7405ffa7, 0xf335faaf, 0xef97f8ef, 0xfff7ebaf}, {-761516, 446724} },
	{83, {0xffefea02, 0x64cfe0f7, 0xc3ca81a6, 0xeafef9ff, 0x641f2020, 0x00822062, 0xa6a7a202, 0xc7fff737}, {-704938, 448907} },
	{61, {0x6d47ffff, 0xb55165d1, 0xd900d0c7, 0xffd9f137, 0x209f29aa, 0x7adbe9b4, 0x0e41024b, 0x7fff2fff}, {-750992, 437234} },
	{47, {0xfafffe76, 0xd337f82e, 0xdb90cfee, 0xe07eb2da, 0xc39bf78c, 0xbaa58099, 0xfb6dbe48, 0xdfdaec10}, {-689902, 418533} },
	{ 8, {0xef8beeff, 0xdf53c4bb, 0xc407a08e, 0xa09d620a, 0x4b31e027, 0xf3972e37, 0x65056505, 0xd5f3f277}, {-628832, 504593} },
	{25, {0x217f279f, 0x15dd859d, 0xa28f031b, 0x3ad9013d, 0x5009773f, 0x388c78f9, 0x6fc12fbe, 0xefd9f9ef}, {-681400, 419287} },
	{ 1, {0x72f3fcd3, 0xfc53d4d5, 0xc88281a0, 0xea4200c6, 0xfb0f8db7, 0x9398aa00, 0xc183cfb9, 0xf9e7e3d5}, {-581482, 561791} },
	{35, {0xfdf5afaf, 0xfd45a79d, 0xfd67a2bf, 0xf1dd809d, 0xb120bfbf, 0xd4d1b954, 0xb9dcaaab, 0xfeefa8bf}, {-639024, 547100} },
	{ 9, {0xaaceeefe, 0xfef6cfdf, 0xfd489d7c, 0xfdfce3aa, 0x339737f3, 0xf4f44eaf, 0xfbfffffe, 0xffffeff3}, {-652627, 485768} },
	{71, {0x55dcf7cc, 0x5c9dbedf, 0x5943af7f, 0xdd79a38f, 0xdcfdb38d, 0xddade1fa, 0xfbe5eacf, 0xffefb2db}, {-681954, 410804} },
	{73, {0xf95bd773, 0x6050d186, 0xb9118120, 0xa921d31a, 0xf1dfebf9, 0xeedad095, 0xf1b33232, 0xd0b3f0f3}, {-584870, 501955} },
	{ 3, {0xe4e1aee7, 0xe2f10aef, 0xfd258f4d, 0xeb370703, 0xf3b7f077, 0x6d9b2725, 0xfe4aea13, 0xefdb6fcb}, {-583765, 455553} },
	{79, {0xd3ffabff, 0xd5d5e293, 0xe9c1d71f, 0x7fa7fa89, 0xd8c18adf, 0xf9ae60fb, 0x2bc283af, 0x3a27027f}, {-699599, 369475} },
	{30, {0xfbd7fff3, 0xfff2ff5a, 0xffddff7f, 0xff586f00, 0x7fe56eae, 0xfa101fec, 0xff45f74c, 0xfd411243}, {-496778, 547543} },
	{54, {0xc86b8608, 0x00458283, 0x00220400, 0x2056e28b, 0xe0077527, 0x422ea51c, 0xeb0ab33f, 0xd7fffa8f}, {-664746, 398349} },
	{32, {0xfd675074, 0x775bc7a9, 0xd997f1dc, 0xff13b93f, 0xaf273db4, 0xfcbbe1fb, 0xfbbfd4fc, 0xfdbffffb}, {-608746, 467728} },
	{36, {0xd151f750, 0x5110e914, 0x09f3817f, 0xd595f789, 0x430442cf, 0xef0582eb, 0xea298afb, 0xffdfeaeb}, {-645609, 363864} },
	{75, {0x9d23d030, 0x3302b800, 0xd26fe1d5, 0xd3a1d591, 0xc22fc8a1, 0x8c20a030, 0x532b499b, 0xffeff5ef}, {-526831, 469055} },
	{48, {0xf57fc482, 0x8084cab0, 0xf4e7a0c0, 0xa3c9e8c6, 0x9972c197, 0x00fa6e67, 0x84d7e54f, 0xc1abd7ff}, {-492602, 516479} },
	{17, {0xfefbf6ff, 0xffe2e6f3, 0xfaf2eadb, 0x3242ff3b, 0xe623c0ae, 0xfff7d4ff, 0xf4509fdf, 0xf563f3df}, {-400263, 595383} },
	{80, {0xc8b7fcb0, 0xe484e162, 0xec00cd20, 0xed2ad38c, 0xc0c5f0b0, 0xb82720aa, 0xc0a9a1df, 0xfd7bfaaf}, {-664214, 367579} },
	{74, {0xba149e00, 0x70800900, 0xa90255fd, 0xf1180119, 0x268600bc, 0x99080588, 0xafe01aaf, 0xbb2be23f}, {-492208, 512186} },
	{53, {0xe5cbe72a, 0x02a4d0a9, 0xa1375020, 0x71558087, 0xf0933372, 0x00a9c091, 0xf282f38a, 0xdfcfafcb}, {-555099, 448537} },
	{ 0, {0x73f3f7ff, 0xf1e3ffef, 0x43f8e1a0, 0xb3e8bb7f, 0xfffdf7df, 0xf5f7ffff, 0xf7e73ddd, 0xe5f4f9fd}, {-482402, 533697} },
	{40, {0xdb5ed7dc, 0xfd50c1c6, 0x7df653de, 0x7562e24e, 0xf3bfb2ed, 0xfffbb16b, 0xbdaaffcc, 0xfaceeecc}, {-701582, 348298} },
	{78, {0xffcf955d, 0x5507770d, 0xeb8f7bb7, 0x70dbf7ff, 0xfa57bc54, 0x59afbf29, 0xafaffa88, 0xffdfbb3f}, {-491828, 489272} },
	{12, {0x6517ada7, 0xcfd7034f, 0xf1032917, 0x2fdd200a, 0xb30046ff, 0xcbfd23fb, 0xbfe3fbbf, 0xeff76f2b}, {-557292, 436954} },
	{58, {0x80ae80d0, 0xfd17201c, 0xba0800df, 0x9d2d087f, 0xe98da8f2, 0xbbab881d, 0x2b2c3b9b, 0x2b0b08bb}, {-521999, 454403} },
	{51, {0xfa60f9fe, 0x58ed0903, 0x4d704b8d, 0xf3dc5209, 0x707490f0, 0xfd0c979d, 0x3054b8e7, 0xfbfbaaaf}, {-635432, 342918} },
	{90, {0xd7fff353, 0x4023d191, 0xf7cd7250, 0xb133d356, 0xfd0f96af, 0xeda50581, 0x59aa14f0, 0xd163f2ff}, {-518384, 426413} },
	{64, {0xe77f67e4, 0xc36ec23e, 0xa00c4400, 0xcc4ea01f, 0xffd5a900, 0x74308e11, 0xf5917108, 0xf747f1bd}, {-597487, 383788} },
	{44, {0x24200020, 0x55330003, 0x7982d61f, 0x7b50140f, 0xa41612b0, 0x98026692, 0x6302eb8b, 0xbfaf20ff}, {-535100, 437221} },
	{14, {0xf877ceff, 0xf0aac35e, 0x0a1530d9, 0xe0408482, 0x150b905f, 0x6a6b8143, 0xab218b43, 0xd4c7fb43}, {-542404, 437408} },
	{22, {0x0f5bdbff, 0x27ee7967, 0x4bc7675f, 0x0006cab2, 0x6c1f82ff, 0xfbaf68e3, 0x7163e1df, 0x3b21df43}, {-692747, 328578} },
	{13, {0xeeffefef, 0xeec697ff, 0xfdef7fe7, 0xedefefee, 0x0fffcfdf, 0x4ff7ffff, 0xf7ff4fff, 0xffffffdf}, {-442164, 510776} },
	{60, {0x7bd577f7, 0xfb013dad, 0xc1027fdf, 0x7f09a28f, 0xdbcedeff, 0xfe42c2ef, 0xfb8d4cdf, 0x79ef408f}, {-591523, 352732} },
	{29, {0xf154df00, 0xf7757adf, 0xf9493105, 0xffc9e9df, 0x6a64bbfb, 0xfbafaeb9, 0xfbf5c982, 0xffcfe2ea}, {-638840, 348900} },
	{84, {0xe74fe50f, 0x072b53bf, 0x56e7ab5e, 0x52d7e1a2, 0xdadf0904, 0x0c6f0514, 0xaacb880c, 0xe63ffe37}, {-531990, 425993} },
	{56, {0x7f4bf3ec, 0x132d719c, 0x185d733c, 0xbddc620e, 0xb431f4b4, 0x50f5c1e8, 0xf0acf3fd, 0xf4edf89f}, {-629348, 324007} },
	{89, {0xd597f77d, 0xc6536185, 0x63153f93, 0xea90dbdb, 0xd9d7d5e1, 0x88cc458b, 0xe47d40a0, 0xe0fbead3}, {-504424, 398098} },
	{77, {0xd15294fa, 0x9844a098, 0x9cc000cc, 0x40045488, 0xd289abef, 0x981f90a8, 0xbb8faa88, 0x010bebcb}, {-564659, 390004} },
	{59, {0x30004478, 0x7554125a, 0x6cb21596, 0x3d7300eb, 0x7aa8a2e6, 0xff9a57fa, 0x0a22b8ff, 0xab9b1a7b}, {-477374, 448100} },
	{63, {0xd3dfe782, 0xc0db84e6, 0xc001e480, 0x74bdc2d9, 0x2251a30b, 0x02029451, 0xb823e28b, 0xf3b7caeb}, {-471746, 460961} },
	{19, {0xf7a2768f, 0xfecaeec6, 0xff7a2fff, 0xff00db20, 0xf7f6ffaf, 0xdfe36f9f, 0xf7fefff7, 0xfb07d655}, {-439826, 495941} },
	{57, {0xdc08e6f8, 0xf9fe066f, 0x3fbc5ffd, 0xfffd3edb, 0xceb89df9, 0xaffc2fb8, 0xbfbcb7bf, 0x3bbb029a}, {-456484, 487925} },
	{ 5, {0xca16ff77, 0x726f7b47, 0x1186fc66, 0x3202f012, 0xe3a79005, 0xdb03a382, 0xe68b52df, 0xef77f0c3}, {-456917, 472795} },
	{81, {0xdf6bf393, 0x1530bf30, 0xf8fc31d0, 0xd9f5d451, 0x7f6fbfbb, 0xc99dfef5, 0xdf5fb150, 0xddd5fdc5}, {-457168, 510230} },
	{41, {0x7345f7ff, 0xffff71c7, 0xaf2fffaf, 0xbfeb83aa, 0xffefffef, 0xffffffff, 0xf1fbaeee, 0xbbefe68a}, {-437853, 495736} },
	{70, {0x4050d7fe, 0x517de103, 0x5ae9019b, 0xf71f080b, 0x4004d9d8, 0xab23db83, 0xf760eade, 0xbbfbeecf}, {-642735, 323611} },
	{16, {0xfaa7f2df, 0xf82fc077, 0x8011d84e, 0x102bc08f, 0x404ee203, 0xd82ec1d7, 0xc464700f, 0xf2ffffab}, {-466974, 423890} },
	{ 2, {0xfa92fe2a, 0xd0022408, 0xd91d8180, 0x74400072, 0x7225bb35, 0x7807c142, 0xa0db5784, 0xf97fd9f9}, {-608977, 355535} },
	{88, {0xffb7f721, 0xcb2af33f, 0x461131e0, 0xea9ab7b6, 0xbb5ff5f3, 0x482fd31f, 0x50ee74b0, 0xe8bfddbb}, {-495007, 391190} },
	{50, {0xf65a2f90, 0xdcc88714, 0x2fb1957d, 0xfd5d311f, 0xca049df0, 0x98000ca8, 0xf3ceceb9, 0xaaa90d99}, {-463708, 424301} },
	{69, {0x3f878311, 0xfdcf0713, 0xf0523d5f, 0xd1276105, 0xe28ba2ef, 0xfc0cc899, 0xf89fc1db, 0xf68f4000}, {-512134, 425991} },
	{67, {0x0d0e4a88, 0xaa06ed00, 0x219b133d, 0x8f8fb05d, 0x3f05bbb1, 0xb501c9e1, 0x2f3f7feb, 0xbf1bdabb}, {-559292, 330277} },
	{52, {0xfffbffff, 0x2c00352c, 0xaef84720, 0x820c73c6, 0x5ff7edf0, 0x9e7eadbe, 0x59d4b380, 0xcdc3decb}, {-390240, 526161} },
	{82, {0xedff2477, 0xf91f7fd7, 0xc9e60d5b, 0xe6dd58cf, 0xe2892035, 0xfb01005f, 0xfa4f485c, 0xf2ffcdef}, {-468148, 456640} },
	{20, {0xd90ffeff, 0x5591f2bb, 0xd319f0f2, 0x5860a2f2, 0xa387fbdf, 0x9fd9e241, 0x85ba58da, 0xc0f9420a}, {-494432, 394044} },
	{24, {0xc21ea0d2, 0xd5c25022, 0x20848511, 0xdd482680, 0x4814c0a1, 0x99dbc091, 0x8c8182bf, 0x33017026}, {-568568, 410527} },
	{39, {0xd6519dd7, 0xfd00a7d5, 0xc95188d9, 0xfd55a8d9, 0x7001b5bd, 0xff81fcc5, 0xf1c52093, 0xe7dcaabb}, {-483315, 419234} },
	{11, {0xefeee5fb, 0xa2ca46e6, 0xc1b7fdbb, 0xfdead7d2, 0x468f8abf, 0x0022d1d5, 0xe8036eb7, 0xc5efdeff}, {-428091, 438439} },
	{65, {0xffffffff, 0xfdffffef, 0xebfffdcc, 0xfdfffdfd, 0xcceddfcd, 0xc8fbeffa, 0xbca7fd88, 0xc8afc6fb}, {-384395, 463287} },
	{68, {0x3bcf7435, 0xfee60e1f, 0x288c7b9d, 0xfcbcff1d, 0xaf8bbbb5, 0xa1a3af0b, 0x98aa9ffd, 0xbef8bb5f}, {-419293, 436218} },
	{92, {0xbe77b880, 0x50f9dc4e, 0x88d10c88, 0x8c5bf31b, 0xa403d288, 0x1611b057, 0x8a2d8888, 0x88abc2ba}, {-566999, 308495} },
	{85, {0xe5fff480, 0x40ed9ba7, 0xa2cec4c6, 0xcbece39f, 0x44abd4c0, 0x4004e887, 0xe08f20a0, 0xd1f7f2ff}, {-568851, 326706} },
	{18, {0x33f94b99, 0x2de740eb, 0x808149ed, 0x2725496d, 0x7e0809da, 0xede880d4, 0x41c0c8c2, 0x272a21af}, {-813074, 232106} },
	{72, {0xfbcbcc82, 0xf0ab83bb, 0x8600328f, 0xc053a2b7, 0xb902a424, 0x41033001, 0xf74390ca, 0xb337c05b}, {-473111, 391803} },
	{49, {0x13cefecc, 0x7ccea006, 0x5e38818e, 0xff8e0e0f, 0xc3ac5924, 0xc0024235, 0xe3cefb2c, 0xcfb8f458}, {-564671, 332378} },
	{38, {0xc202cede, 0xfeddc2d0, 0x8846c09b, 0x7a528206, 0xe0aa4adc, 0xde06307a, 0x3140929f, 0xd3ff4afb}, {-499509, 377517} },
	{26, {0xaeebae97, 0xcabefead, 0xaafeaeb1, 0x8f4d56ec, 0xffff9c07, 0xbebf7efd, 0xbba7ef32, 0xdbfed7ed}, {-418579, 462609} },
	{43, {0x5b720430, 0x7b43000a, 0x55020114, 0x75320210, 0x004340c0, 0x0c840c00, 0x584161ce, 0x5f43733b}, {-607001, 307939} },
	{28, {0xbb5f2337, 0xd1d015d4, 0x09f52b9d, 0xd5d181b0, 0xd0c4b19f, 0xd8c06850, 0xa8b2899c, 0xdf409844}, {-496819, 378247} },
	{86, {0xd4dfc5a7, 0x11dd91d5, 0x0949c45f, 0xe9c9cd55, 0x0d953f2b, 0x27dc1467, 0x0bbd250e, 0xaf17e01f}, {-518181, 351092} },
	{31, {0xea08e482, 0xde824c2e, 0xcec0a442, 0xf18bf4e7, 0x4054404a, 0x68000908, 0x488946ff, 0xffe74b9f}, {-512495, 350054} },
	{55, {0xf17fffee, 0xc0f0ed86, 0xf87c8500, 0xfdd0e34a, 0xd0cb2004, 0xc0e00298, 0xfb45d594, 0xf1d7a78f}, {-445075, 395064} },
	{91, {0xf7bfa713, 0xb7573c71, 0xf2bfe7d7, 0x30717afd, 0xff7fa530, 0xf7ffff5e, 0xfd7effbc, 0xfb7ffd7f}, {-372041, 474957} },
	{15, {0xfa5ff975, 0x343eb95e, 0x160dc2f6, 0x1220b052, 0x58b9c41e, 0x09bdc013, 0xf4eccc62, 0xf947f442}, {-470943, 369500} },
	{21, {0xeca7d2f3, 0xe0ddd033, 0xd98cca80, 0xb81eda44, 0x8a4b6d53, 0xcb2108b6, 0x4d0bbb02, 0xfae9b3f0}, {-498566, 369657} },
	{45, {0xd6a0d2d4, 0xfe8c805f, 0xd080d85f, 0x799008b8, 0x948c209c, 0xf84d248c, 0x4bc874bb, 0xde188bbe}, {-461305, 391924} },
	{ 6, {0x524ec3ea, 0xf1ce2174, 0xd426aa0a, 0xb8cccce0, 0xd203a153, 0xb34308d1, 0xb0fb81f3, 0xd565c271}, {-726108, 258662} },
	{42, {0x004105bf, 0x8f1649d5, 0xefc407b3, 0xf5c52084, 0xa0809afd, 0x99c08180, 0xd5b022bb, 0xc2c89449}, {-557952, 323418} },
	{66, {0xdc81ea18, 0x10048210, 0x15002854, 0x5511a39b, 0xc80d84d4, 0x01002d11, 0xbbfb7bef, 0xab0b28fb}, {-454313, 434051} },
	{46, {0x2cb70e19, 0x56309213, 0x7d297df7, 0xf5dc1bdd, 0xcafea737, 0xbe078ab0, 0x60d79313, 0xff6ff4fa}, {-426722, 459126} },
	{87, {0xdad6c098, 0x04245194, 0x00910c59, 0x0c936392, 0xb183c488, 0xc8cca82a, 0xa8a09888, 0x2a17f6bf}, {-563427, 346554} }
};

static ROMDATA Lbp_Rect_t FACADE_RECT[FACADE_RECT_CNT]={
	{ 0, 0, 8, 8}, { 0, 1, 7, 7}, { 0, 2, 3, 7}, { 0, 3, 3, 4},
	{ 0, 8, 4, 3}, { 0, 9, 3, 5}, { 0,11, 4, 2}, { 0,11, 8, 4},
	{ 0,14, 4, 3}, { 1, 0, 5, 3}, { 1, 1, 6, 3}, { 1, 2, 2, 2},
	{ 1, 2, 3, 4}, { 2, 0, 3, 2}, { 2,12, 2, 2}, { 2,14, 2, 3},
	{ 2,17, 3, 2}, { 2,18, 3, 2}, { 3, 1, 3, 5}, { 3, 4, 2, 2},
	{ 3,12, 3, 4}, { 4, 0, 5, 7}, { 4, 1, 3, 6}, { 4, 3, 3, 4},
	{ 4, 6, 2, 1}, { 4, 7, 5, 2}, { 5, 1, 2, 1}, { 5, 6, 2, 5},
	{ 5, 7, 4, 2}, { 5, 8, 2, 3}, { 5,13, 3, 1}, { 5,18, 2, 1},
	{ 6, 1, 6, 3}, { 6, 3, 4, 4}, { 6, 4, 4, 3}, { 6, 5, 4, 3},
	{ 6, 6, 2, 5}, { 6, 7, 4, 4}, { 6,20, 2, 1}, { 7, 3, 4, 4},
	{ 7, 4, 3, 2}, { 7, 5, 3, 2}, { 7, 7, 4, 2}, { 8,14, 1, 1},
	{ 8,18, 1, 1}, { 8,21, 1, 1}, { 9, 0, 2, 1}, { 9, 2, 2, 6},
	{ 9,11, 1, 1}, { 9,12, 2, 1}, { 9,16, 2, 1}, {10, 6, 3, 5},
	{10, 7, 1, 5}, {10, 9, 1, 1}, {10,10, 1, 1}, {10,10, 1, 2},
	{10,10, 2, 2}, {10,17, 1, 1}, {10,17, 2, 1}, {10,18, 1, 1},
	{11, 3, 2, 4}, {11, 5, 3, 3}, {11, 7, 4, 4}, {11,10, 1, 1},
	{11,10, 1, 2}, {11,13, 4, 1}, {11,15, 2, 1}, {11,17, 2, 2},
	{12, 0, 3, 3}, {12, 3, 1, 3}, {12, 6, 2, 5}, {12, 8, 3, 3},
	{12,11, 1, 1}, {12,14, 4, 3}, {12,15, 1, 1}, {12,18, 2, 2},
	{13, 7, 3, 4}, {13,14, 2, 2}, {14, 0, 3, 3}, {14, 4, 1, 3},
	{14, 5, 3, 6}, {14,18, 2, 2}, {15, 0, 2, 2}, {15, 1, 3, 6},
	{15, 2, 3, 3}, {15, 2, 3, 7}, {15, 3, 3, 4}, {15, 7, 2, 1},
	{15,10, 3, 4}, {15,12, 3, 4}, {15,17, 2, 2}, {16, 0, 2, 1},
	{16, 7, 2, 1}
};


// LBP CHARACTER(EYE) DBASE (18x9)
#define CHARAC_HSIZE 18
#define CHARAC_VSIZE 9
#define CHARAC_STAG_CNT 5
#define CHARAC_NODE_CNT 53
#define CHARAC_RECT_CNT 47

static ROMDATA Lbp_Stag_t CHARAC_STAG[CHARAC_STAG_CNT]={
	{ 0,  4,  -477127},
	{ 4,  7, -1220421},
	{11, 11,  -628841},
	{22, 12, -1078783},
	{34, 19, -1079198}
};

static ROMDATA Lbp_Node_t CHARAC_NODE[CHARAC_NODE_CNT]={
	{13, {0xf54bf3ef, 0xfd9f208d, 0x17000088, 0xfd9d0808, 0xb98aa8bb, 0xf9af8089, 0x88908a8e, 0xff8b828a}, {-938592, 839886} },
	{ 7, {0xd717a9bf, 0xfdd70199, 0x1105009b, 0x1fd5001b, 0xcaab8abb, 0xf99c8082, 0xaa8b8bbb, 0x7bef01bf}, {-893436, 819509} },
	{30, {0xf3d9d1ff, 0xffdf91dd, 0xd100d09f, 0xdf95909f, 0x34cb20eb, 0xdf8a288b, 0x7b9f009f, 0x3bcf03cf}, {-877551, 703610} },
	{ 6, {0x7d3ff7ff, 0x13bb901a, 0x5111d0df, 0x190b219f, 0xefabf9fb, 0x398980a8, 0xda8bb1ff, 0x2bcfabbf}, {-934124, 651291} },
	{23, {0xfd83e2aa, 0xf395d8c9, 0xf821c042, 0xff5ff1cf, 0xf682fcff, 0xf9838182, 0xfb83dfaf, 0xffdffbff}, {-836031, 767866} },
	{33, {0xfacfea0a, 0xecab818e, 0xe0028000, 0xe20fd22b, 0xf386d720, 0xf102a482, 0xc2c3b303, 0xf71fea8b}, {-778991, 630309} },
	{10, {0xf7ffafff, 0xd947a9f7, 0x9c97a03b, 0xffdfa23f, 0xb543bdff, 0xf327d6b5, 0xf7d522bb, 0xfbffabbf}, {-727345, 633572} },
	{31, {0xf3dbf32a, 0xf5d7458a, 0x91018328, 0xef8f8006, 0x919b9218, 0xd2808010, 0xe39b8120, 0xffefea00}, {-824686, 541956} },
	{37, {0xf6efe2cb, 0x18d1885b, 0xc012f809, 0xc0c7e38f, 0x60410003, 0x10200000, 0xb083e2eb, 0xf06fd2db}, {-652750, 584420} },
	{ 9, {0xeb57f33f, 0x3111f113, 0x754291f7, 0x1393b592, 0xe0c0d316, 0xbe17c8f6, 0xd34fbd9b, 0x3f3b3fbb}, {-895481, 421637} },
	{18, {0xd0a3f702, 0xd0a5feb2, 0xf777f441, 0xfdfff646, 0xb62db523, 0xffffeffe, 0xe6f7d702, 0xefffb5fb}, {-592472, 668268} },
	{28, {0x60b742ff, 0xffdd5a55, 0x9b58c7fd, 0x9dc3dc0f, 0x201e04ff, 0xfbdd22eb, 0x24abfbbf, 0xffcffdbf}, {-753514, 648372} },
	{21, {0xf767ffff, 0xffebd1df, 0xef01d107, 0xb367f4db, 0xf10b939b, 0xca89c0bb, 0xffe1e3eb, 0xa0f2e6e3}, {-699092, 587222} },
	{39, {0xfdcfffff, 0xdbbf10d3, 0xd139d79f, 0xdfcfd0d5, 0xf704b898, 0xf99f8883, 0xd89301fc, 0xfffd1070}, {-612809, 630761} },
	{ 4, {0x25397711, 0x12b13030, 0xd911995f, 0x3fd4b895, 0x7e9bff9f, 0xbbf9bafb, 0xfbe4fb8b, 0xffffc8bb}, {-688795, 557489} },
	{27, {0xcecfee82, 0xacc7850a, 0xcc008a80, 0xe0eac0aa, 0xd104a38a, 0xca008042, 0xd101a113, 0xe7dfe2ce}, {-680710, 476442} },
	{ 1, {0xf9d5b1bf, 0x73d5151d, 0xcdd08abb, 0xeddf2084, 0xf32dfdbf, 0xf8e0cb5d, 0xdbf3adbf, 0xffdc273b}, {-655267, 490045} },
	{35, {0xd180ffff, 0xdf139077, 0xc000c57f, 0xff9d085e, 0xb80093f4, 0xdaa00080, 0x6b4127e7, 0xdf815351}, {-686087, 428695} },
	{19, {0x514ff2ff, 0xd5010150, 0x11119117, 0x19002012, 0xdb8382bf, 0xd9ae0a92, 0xea10e999, 0xaa98a8ea}, {-604354, 487771} },
	{36, {0xf9d1e7cf, 0xdc4310b1, 0xc001504d, 0x7109c045, 0x61233028, 0x754b8042, 0x738af1bf, 0x7557ea8b}, {-698972, 402798} },
	{14, {0xe8c9a4ff, 0xc110a17d, 0xdd442819, 0xf95bad25, 0x8811921d, 0xa8182180, 0x82c4b233, 0xbb9d95df}, {-676854, 449987} },
	{20, {0x80a68122, 0x1b958801, 0x1310c851, 0xfddbc09f, 0x138db12a, 0x31800088, 0x71830120, 0xf78b818b}, {-738407, 415502} },
	{16, {0xdfdfdfff, 0xfff1a79f, 0xf1fc53ee, 0xfbfef7cc, 0xdb3bb9ff, 0xbdf99dff, 0xf3fbffe8, 0xffdeffbc}, {-736289, 552056} },
	{32, {0xefffee22, 0xdfbbd7bd, 0xeaf5ae02, 0xeebbf2c9, 0xf3ffb707, 0xfbebacfb, 0xfff5f740, 0xfffffbaf}, {-750612, 414576} },
	{41, {0x0bdf0ecf, 0x3ff5bfff, 0xd539b7ff, 0xffff71db, 0xffff6d9f, 0xfbfd68fa, 0xfdcdfcaf, 0xffdf7faf}, {-702202, 456266} },
	{12, {0x3f5d97bf, 0x031b3f10, 0xfd945d09, 0x0fbcae19, 0xef9eb3bf, 0x87878ed7, 0xbe27a9fb, 0x888facb1}, {-664618, 428605} },
	{42, {0x4073005d, 0x7ad64bdf, 0xe8a39bdf, 0xa8b2a02b, 0xff952aeb, 0xfba165e7, 0xfa82ef9f, 0xfb22f3cf}, {-634080, 448639} },
	{25, {0xfbdfdfa7, 0x0045c30f, 0xd52082c2, 0x7fcf7183, 0x838681af, 0x00208410, 0x6189c20f, 0x45fb8be3}, {-657503, 409299} },
	{34, {0xfffff71f, 0xe58b0daf, 0xec7b05b7, 0xe5df8093, 0xda8df2ff, 0xafba04b0, 0x50711260, 0xdffdf460}, {-680687, 345085} },
	{ 2, {0xca82dd6a, 0xcf32c1c6, 0xcd2199f7, 0xcfeeb0c0, 0xbc0cebba, 0xf82a8c97, 0x3a412f9e, 0xff374455}, {-547326, 420486} },
	{18, {0x8020b731, 0xbdded096, 0x502f8135, 0xdc9dba83, 0xf827b3b1, 0x8d808592, 0x42894123, 0xcde726e7}, {-566793, 424624} },
	{29, {0xded9c382, 0x448fb1cc, 0xcc7b8606, 0xcacfa6af, 0xe9d3f093, 0x43e2a681, 0xeacd834b, 0xdec7f2f3}, {-590921, 421472} },
	{46, {0x7d7ed8d5, 0xfdfd0edf, 0x7fbffd7f, 0x994d5d9d, 0xfade8cd4, 0xbefe7ffc, 0x9cfcae68, 0x9a3a9e7a}, {-436718, 585610} },
	{11, {0x11243713, 0x3bad5838, 0x7f01b191, 0xdd9d8383, 0x34e68fb1, 0x398980ec, 0xf38913e7, 0xfbcff8b9}, {-741976, 351441} },
	{22, {0xf5d3e1a6, 0xd1d5d073, 0xd165d9e2, 0xf5d551ef, 0xf110f1f7, 0x90185430, 0xf1f0fbaf, 0xf7fffaff}, {-671766, 377049} },
	{ 7, {0x7889fdfd, 0xd3408dc4, 0xffc231dd, 0xfffca1dd, 0xd404d4dc, 0x4440a048, 0xfd56bbbf, 0xf5d9bbdf}, {-412916, 538033} },
	{37, {0xfddfeecf, 0xc298d0a6, 0xb8a3d44f, 0xc285d589, 0x70326007, 0x44276257, 0xf487efeb, 0x6763b3cf}, {-660290, 339475} },
	{ 8, {0x80a012bc, 0xf99a03b3, 0xd92097fd, 0xfd04664c, 0x9400a7b0, 0xf80ba4a9, 0xba00a09c, 0xbf334801}, {-493363, 414201} },
	{24, {0xfddfc587, 0x018bc1a7, 0xd525d351, 0xf5d5e5c7, 0x14088882, 0x22c28504, 0xe280428b, 0xd729420a}, {-499260, 372903} },
	{42, {0xe0fe40c6, 0xcaa90070, 0xcab0dd8b, 0x02de20ad, 0xd9e1ecd4, 0xfb6b0c16, 0xa38ad3db, 0xeafbf9be}, {-552634, 330946} },
	{15, {0x289c223f, 0xf554409d, 0xddd02015, 0xcfc58c79, 0x5000b09c, 0x14c184c8, 0x2a0033bb, 0xb9cdbed0}, {-670542, 295506} },
	{17, {0x98404835, 0x50004004, 0x0101105f, 0x11545a19, 0x910196ba, 0xd809cc05, 0x8808995b, 0x8a0afcbb}, {-463856, 412260} },
	{43, {0xde54d17c, 0x3e340080, 0x13a151d5, 0x5df806d5, 0x808099b0, 0xbf40031f, 0xa888f099, 0xaaaa1a3f}, {-471643, 392100} },
	{38, {0xde7fe707, 0xf1118110, 0xd80000e2, 0xb95530a1, 0xf0038201, 0x31230a63, 0x2701b35b, 0xf222c01a}, {-526851, 362364} },
	{ 0, {0xeefacef4, 0xbfeca6f8, 0xfbfcfff5, 0xfd6c06ac, 0xaeffa7f1, 0xefff7ff4, 0xf6f47fff, 0x2f0b2703}, {-412829, 457666} },
	{39, {0x7383b35f, 0xbf8bc1a9, 0xc041916f, 0xff99300c, 0x8a0a0b9c, 0x880a80fb, 0x038c05d4, 0xddde5114}, {-597620, 308165} },
	{40, {0x00da2858, 0x38fe0256, 0xd5810db9, 0x9500925d, 0x1cc558b7, 0x1fd9418a, 0x6d83cbbf, 0x3b0338ff}, {-715579, 242085} },
	{26, {0xd74be38b, 0x005e828b, 0xf08a4101, 0xd7d55275, 0x921b4b87, 0x00c8008c, 0x178383a2, 0xc18780cf}, {-538908, 342823} },
	{ 3, {0xa9d9ab3b, 0xd544f121, 0xa9019a30, 0x78800091, 0x9a10821f, 0x2a007080, 0xcf00ab9b, 0xea9851d0}, {-530022, 340602} },
	{13, {0x5a414d67, 0x30015452, 0xc401cccc, 0x7120d128, 0x001180e5, 0x11109011, 0xf319c1ee, 0x75ebe28a}, {-765225, 215102} },
	{44, {0x7c40019c, 0xdc4a2180, 0x54914159, 0x9d9c2589, 0xc018b0ec, 0x3282e1d0, 0x8242a9ce, 0x3e8c88ce}, {-535546, 319354} },
	{45, {0xfe3f4bd5, 0x9efceed8, 0xfffe7fde, 0x5f7b5f9d, 0xfdfebc4c, 0xbd7fbfff, 0xfc77ff90, 0xa2bbb2bb}, {-379529, 517118} },
	{ 5, {0xca0a0444, 0xaa511c19, 0x5801d0ff, 0x5c944202, 0x1a8a48c8, 0x49a00282, 0x8b8a6919, 0xdedfdaaa}, {-505603, 375050} }
};

static ROMDATA Lbp_Rect_t CHARAC_RECT[CHARAC_RECT_CNT]={
	{ 0, 0, 2, 1}, { 0, 0, 5, 3}, { 0, 1, 1, 1}, { 0, 2, 5, 2},
	{ 0, 3, 4, 2}, { 0, 6, 1, 1}, { 1, 0, 3, 3}, { 1, 0, 5, 3},
	{ 1, 1, 1, 1}, { 2, 0, 3, 3}, { 2, 0, 5, 3}, { 2, 3, 2, 2},
	{ 2, 3, 3, 1}, { 3, 0, 4, 2}, { 3, 0, 5, 3}, { 3, 2, 5, 2},
	{ 4, 0, 3, 1}, { 4, 1, 1, 1}, { 4, 6, 1, 1}, { 5, 0, 1, 1},
	{ 5, 5, 1, 1}, { 6, 0, 1, 2}, { 6, 2, 2, 1}, { 6, 3, 2, 1},
	{ 6, 4, 1, 1}, { 6, 5, 1, 1}, { 6, 6, 1, 1}, { 7, 3, 1, 2},
	{ 7, 3, 3, 2}, { 7, 6, 1, 1}, { 8, 0, 3, 3}, { 8, 1, 1, 1},
	{ 8, 5, 1, 1}, { 8, 6, 1, 1}, { 9, 1, 1, 1}, { 9, 1, 2, 1},
	{ 9, 4, 1, 1}, { 9, 5, 1, 1}, { 9, 6, 1, 1}, {10, 0, 1, 1},
	{11, 3, 2, 1}, {11, 3, 2, 2}, {11, 6, 1, 1}, {14, 1, 1, 1},
	{14, 2, 1, 1}, {15, 0, 1, 1}, {15, 1, 1, 1}
};

//loop count for the scale
#define SCALE_ITERATION		10

//search step size
#define STEP_H_RATIO		10

//eye detection in the face area
#define C_LOOP_H_CNT (FACADE_HSIZE-CHARAC_HSIZE)
#define C_LOOP_V_CNT (FACADE_VSIZE/3)

// ----------------------------------------------------------------------
// Static Global Data section variables
// ----------------------------------------------------------------------
VBYTE fFDCtrlProcCtrl = 0x00;
static FD_CTRL_PARAM stFDCTRL;
static FD_ITER_PARAM stFDITER;

//LBP database
static ROMDATA Lbp_Base_t FACADE_BASE={
	FACADE_STAG_CNT,
	FACADE_NODE_CNT,
	FACADE_RECT_CNT,
	FACADE_STAG,
	FACADE_NODE,
	FACADE_RECT};

static ROMDATA Lbp_Base_t CHARAC_BASE={
	CHARAC_STAG_CNT,
	CHARAC_NODE_CNT,
	CHARAC_RECT_CNT,
	CHARAC_STAG,
	CHARAC_NODE,
	CHARAC_RECT};

//scale table
#define SCALE_QF (1<<3)  //Q3 format
static ROMDATA BYTE SCALE_TBL[SCALE_ITERATION]={
	 8,  // x1.0
	10,  // x1.25
	12,  // x1.5
	14,  // x1.75
	16,  // x2.0
	18,  // x2.25
	20,  // x2.5
	24,  // x3.0
	32,  // x4.0
	40,  // x5.0
};

//vertical shift table => step_size(scale*shift)가 정수가 되도록 설정
static ROMDATA BYTE SHIFT_TBL[SCALE_ITERATION]={
	2,
	4,
	2,
	4,
	2,
	4,
	2,
	2,
	2,
	2,
};

//integral image buffer
static DWORD integ_buff[FD_FRAME_HSIZE*FD_FRAME_LSIZE];  //bh 2013.12.10 fd
#if FD_BILINE_READ
static DWORD integ_temp[FD_FRAME_HSIZE];
#endif

// ----------------------------------------------------------------------
// External Variable 
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Static Prototype Functions
// ----------------------------------------------------------------------
static void MISP_IntegRead(DWORD* line_buff, WORD pos_y);

// ----------------------------------------------------------------------
// Static functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
static void Init_FD_PARAM(PFD_CTRL_PARAM pCTRL) {
	return;
}

//--------------------------------------------------------------------------------------------------------------------------
static void Init_FD_REGISTER(PFD_CTRL_PARAM pCTRL) {
	return;
}

//--------------------------------------------------------------------------------------------------------------------------
void Init_FD(void) {
	memset(&stFDCTRL, 0, sizeof(FD_CTRL_PARAM));
	memset(&stFDITER, 0, sizeof(FD_ITER_PARAM));
	Init_FD_PARAM(&stFDCTRL);
	Init_FD_REGISTER(&stFDCTRL);
	return;
}

//--------------------------------------------------------------------------------------------------------------------------
static void FD_ReadLine(DWORD* integ_line, const WORD pos_y, const WORD mod_y) {
#if FD_BILINE_READ
	WORD i;
	if(mod_y) {  //read 2-line
		MISP_IntegRead(integ_line, pos_y+0);
		MISP_IntegRead(integ_temp, pos_y+1);

		//bilinear filter
		for(i=0; i<FD_FRAME_HSIZE; i++)
			integ_line[i]=(integ_line[i]*(SCALE_QF-mod_y)+integ_temp[i]*mod_y)/SCALE_QF;
	}
	else {  //read 1-line
		MISP_IntegRead(integ_line, pos_y);
	}
#else
	//read 1-line
	MISP_IntegRead(integ_line, pos_y);
#endif
	return;
}

//--------------------------------------------------------------------------------------------------------------------------
static void Lbp_UpdateBuff(
	const WORD pos_y,  //first line of window
	const BYTE scale,  //scale factor (x10)
	const BYTE shift,
	DWORD* LineBuff) {

	BYTE i;
	WORD scl_off;

	if(pos_y==0) {
		//update all(0~25) line
		for(i=0; i<FD_FRAME_LSIZE; i++) {
			scl_off = (WORD)(i*scale);
			FD_ReadLine(LineBuff+i*FD_FRAME_HSIZE, pos_y+(scl_off/SCALE_QF), (scl_off%SCALE_QF));
		}
	}
	else {
		//shift buffer
		for(i=0; i<FD_FRAME_LSIZE-shift; i++)
			memcpy(LineBuff+i*FD_FRAME_HSIZE, LineBuff+(i+shift)*FD_FRAME_HSIZE, sizeof(DWORD)*FD_FRAME_HSIZE);

		//update last #n line
		for(i=FD_FRAME_LSIZE-shift; i<FD_FRAME_LSIZE; i++) {
			scl_off = (WORD)(i*scale);
			FD_ReadLine(LineBuff+i*FD_FRAME_HSIZE, pos_y+(scl_off/SCALE_QF), (scl_off%SCALE_QF));
		}
	}

	return;
}

//--------------------------------------------------------------------------------------------------------------------------
#define Lbp_IntegSum(a,b,c,d) ((a)+(d)-(b)-(c))  //A+D-B-C
/*
//read integral sum with a, b, c, d
static DWORD Lbp_IntegSum(
	const DWORD* frame,
	const WORD pos_a,
	const WORD pos_b,
	const WORD pos_c,
	const WORD pos_d) {

	
	return (DWORD)(frame[pos_a]+frame[pos_d]-frame[pos_b]-frame[pos_c]);
}
*/

//--------------------------------------------------------------------------------------------------------------------------
static void Lbp_DetectFacade(
	const Lbp_Base_t* lbp_base,
	const DWORD* integral_sum,  // 25-line integral 32bit frame
	const WORD pos_x,  //start x position
	const BYTE scale,  //scale factor (x10)
	BOOL *detected) {

	//stage iteration
	BYTE i_stag;
	BYTE i_node;

	BOOL stag_pass;
	LONG stag_sum;

	ROMDATA Lbp_Stag_t* p_cur_stag;
	ROMDATA Lbp_Node_t* p_cur_node;
	ROMDATA Lbp_Rect_t* p_rect;

	WORD scale_off_x;
	WORD scale_width;
	WORD lbuff_off_y;
	WORD lbuff_heigh;

	DWORD read_val[16];

	DWORD integ_center;
	BYTE  calc_lbp_msb;
	BYTE  calc_lbp_lsb;

	DWORD sub_set_data;
	DWORD mask_bit_pos;

	//stage iteration
	stag_pass=TRUE;
	p_cur_stag=lbp_base->p_lbp_stag;
	for(i_stag=0; i_stag<lbp_base->stag_cnt; i_stag++, p_cur_stag++) {

		//node iteration
		stag_sum=0;
		p_cur_node=lbp_base->p_lbp_node+(p_cur_stag->node_pos);
		for(i_node=0; i_node<p_cur_stag->node_cnt; i_node++, p_cur_node++) {
			//rect position/size
			p_rect = lbp_base->p_lbp_rect+(p_cur_node->rect_no);
			scale_off_x = pos_x+(WORD)(p_rect->x*scale/SCALE_QF);
			scale_width = (WORD)(p_rect->w*scale/SCALE_QF);
			lbuff_off_y = (WORD)(p_rect->y*FD_FRAME_HSIZE);
			lbuff_heigh = (WORD)(p_rect->h*FD_FRAME_HSIZE);

			//read integral data
			//  0  1  2  3
			//  4  5  6  7
			//  8  9 10 11
			// 12 13 14 15
			DWORD* p_buff_offs=(DWORD*)integral_sum+lbuff_off_y+scale_off_x;
			DWORD* p_buff;
			p_buff=p_buff_offs+(0*lbuff_heigh);
			read_val[ 0]=*p_buff;  p_buff+=scale_width;
			read_val[ 1]=*p_buff;  p_buff+=scale_width;
			read_val[ 2]=*p_buff;  p_buff+=scale_width;
			read_val[ 3]=*p_buff;
			p_buff=p_buff_offs+(1*lbuff_heigh);
			read_val[ 4]=*p_buff;  p_buff+=scale_width;
			read_val[ 5]=*p_buff;  p_buff+=scale_width;
			read_val[ 6]=*p_buff;  p_buff+=scale_width;
			read_val[ 7]=*p_buff;
			p_buff=p_buff_offs+(2*lbuff_heigh);
			read_val[ 8]=*p_buff;  p_buff+=scale_width;
			read_val[ 9]=*p_buff;  p_buff+=scale_width;
			read_val[10]=*p_buff;  p_buff+=scale_width;
			read_val[11]=*p_buff;
			p_buff=p_buff_offs+(3*lbuff_heigh);
			read_val[12]=*p_buff;  p_buff+=scale_width;
			read_val[13]=*p_buff;  p_buff+=scale_width;
			read_val[14]=*p_buff;  p_buff+=scale_width;
			read_val[15]=*p_buff;

			//center sum
			integ_center =
				(Lbp_IntegSum(read_val[ 5],read_val[ 6],read_val[ 9],read_val[10]));

			//calc lbp (upper 3rect)
			calc_lbp_msb =
				(Lbp_IntegSum(read_val[ 0],read_val[ 1],read_val[ 4],read_val[ 5])>=integ_center? 4:0)|
				(Lbp_IntegSum(read_val[ 1],read_val[ 2],read_val[ 5],read_val[ 6])>=integ_center? 2:0)|
				(Lbp_IntegSum(read_val[ 2],read_val[ 3],read_val[ 6],read_val[ 7])>=integ_center? 1:0);

			//calc lbp (lower 5rect)
			calc_lbp_lsb =
				(Lbp_IntegSum(read_val[ 6],read_val[ 7],read_val[10],read_val[11])>=integ_center?16:0)|
				(Lbp_IntegSum(read_val[10],read_val[11],read_val[14],read_val[15])>=integ_center? 8:0)|
				(Lbp_IntegSum(read_val[ 9],read_val[10],read_val[13],read_val[14])>=integ_center? 4:0)|
				(Lbp_IntegSum(read_val[ 8],read_val[ 9],read_val[12],read_val[13])>=integ_center? 2:0)|
				(Lbp_IntegSum(read_val[ 4],read_val[ 5],read_val[ 8],read_val[ 9])>=integ_center? 1:0);

			//select leaf value
			//msb 3bit : subset selection 에 사용
			//lsb 5bit : subset(32bit)과의 mask position 설정
			sub_set_data = p_cur_node->node_data[calc_lbp_msb];
			mask_bit_pos = 1<<calc_lbp_lsb;
			if(sub_set_data & mask_bit_pos)
				stag_sum += p_cur_node->leaf_val[0];
			else
				stag_sum += p_cur_node->leaf_val[1];
		}

		//check cascade
		if(stag_sum < p_cur_stag->stag_thr) {
			stag_pass=FALSE;
			break;
		}
	}

	//result
	*detected = stag_pass;

	return;
}


//--------------------------------------------------------------------------------------------------------------------------
static void Lbp_DetectFace(
	const DWORD* integral_sum,  // 25-line integral 32bit frame
	const WORD pos_x,  //start x position
	const BYTE scale,  //scale factor (x10)
	BOOL* detect_face) {

	BYTE i, j;
	BOOL detect_facade;
	BOOL detect_charac;

	//detect face
	Lbp_DetectFacade(
		&FACADE_BASE,
		integral_sum,
		pos_x,
		scale,
		&detect_facade);

	//if face exist, detect eye
	if(detect_facade) {
		for(i=0; i<C_LOOP_V_CNT; i++) {
			for(j=0; j<C_LOOP_H_CNT; j++) {
				Lbp_DetectFacade(
					&CHARAC_BASE,
					integral_sum+(i*FD_FRAME_HSIZE),
					(pos_x+(WORD)(j*scale/SCALE_QF)),
					scale,
					&detect_charac);

				if(detect_charac)  //stop x-loop
					break;
			}

			if(detect_charac)  //stop y-loop
				break;
		}
	}
	else {
		detect_charac=FALSE;
	}

	//update result
	*detect_face = detect_charac;

	return;
}

//--------------------------------------------------------------------------------------------------------------------------
static void Lbp_PostProc(
	const WORD pos_x,
	const WORD pos_y,
	const BYTE scale,  //scale factor (x10)
	const BOOL detect_face) {

	if(detect_face) {
		if(stFDCTRL.fd_num<FD_SLOT_NUM) {
			WORD	ratio_h, ratio_v;  //bh 2013.11.28
			ratio_h = (1920/FD_FRAME_HSIZE);	ratio_v = (1080/FD_FRAME_VSIZE);  //bh 2013.11.28

			POS_t* pPos=(POS_t*)(&(stFDCTRL.fd_pos[stFDCTRL.fd_num]));
			pPos->lx = pos_x * ratio_h;
			pPos->ly = pos_y * ratio_v;
			pPos->rx = pPos->lx + (FACADE_HSIZE*scale/SCALE_QF)*ratio_h;
			pPos->ry = pPos->ly + (FACADE_VSIZE*scale/SCALE_QF)*ratio_v;
			UARTprintf("\rFD : X(%d), Y(%d), Width(%d), Height(%d) \r\n",
				pPos->lx, pPos->ly, pPos->rx-pPos->lx, pPos->ry-pPos->ly);

			stFDCTRL.fd_num++;
		}
		else {
			UARTprintf("\rFD : SLOT FULL !!! \r\n");
		}
	}
	return;
}



// ----------------------------------------------------------------------
// Exported functions
// ----------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------
void FD_DetectFace(void)
{
	//init
	if(stFDITER.cont==0) {
		stFDITER.scle=0;
		stFDITER.hori=0;
		stFDITER.vert=0;
		stFDCTRL.fd_num=0;
	}

	//scale iteration
	for(  ; stFDITER.scle<SCALE_ITERATION; stFDITER.scle++) {
		//scale
		BYTE scale = SCALE_TBL[stFDITER.scle];
		BYTE shift = SHIFT_TBL[stFDITER.scle];

		//set window/step size
		BYTE w_size = (BYTE)(FACADE_HSIZE*scale/SCALE_QF);  //window size
		BYTE step_h = (BYTE)(w_size/STEP_H_RATIO);
		BYTE step_v = (BYTE)(shift*scale/SCALE_QF);  //scale*shift

		//vertical iteration
		for(  ; stFDITER.vert<FD_FRAME_VSIZE-w_size; stFDITER.vert+=step_v) {

			//horizontal iteration
			for(  ; stFDITER.hori<FD_FRAME_HSIZE-w_size; stFDITER.hori+=step_h) {
				//update line buffer
				if(stFDITER.hori==0)
					Lbp_UpdateBuff(stFDITER.vert, scale, shift, integ_buff);

				//find 24x24 face in 25x25 window
				BOOL detect_face;
				Lbp_DetectFace(
					integ_buff,
					stFDITER.hori,
					scale,
					&detect_face);

				//post process
				Lbp_PostProc(
					stFDITER.hori,
					stFDITER.vert,
					scale,
					detect_face);

				//seize FD loop
				if(fFDCtrlProcCtrl&FD_ITER_SEIZE) {
					fFDCtrlProcCtrl&=~FD_ITER_SEIZE;
					stFDITER.cont=1;
					return;
				}
	    	}  //end loop stFDITER.hori
			stFDITER.hori=0;
		}  //end loop stFDITER.vert
		stFDITER.vert=0;
	}//end loop stFDITER.scle
	stFDITER.scle=0;
	stFDITER.cont=0;

	return;
}


//--------------------------------------------------------------------------------------------------------------------------
static void MISP_IntegRead(DWORD* line_buff, WORD pos_y) {
	//read integral 320x4byte from MISP
	DWORD e_addr = stFDCTRL.mem_start + (pos_y * stFDCTRL.mem_count);  //y_aux ~ (col_count_per_line*8)
#if REV_BYTE_SWAP
	MISP_MultiRead(MISP_SDRAM_ID, e_addr, (PBYTE)line_buff, FD_FRAME_HSIZE*4);  //1line x 4byte

#else
	WORD i, j;
	BYTE rData[4];
	for(i=0;i<FD_FRAME_HSIZE/2;i++)
	{
		MISP_MultiRead(MISP_SDRAM_ID, e_addr+(i*8+0), rData, 4);  //read 4 byte
		for(j=0;j<4;j++)	((PBYTE)line_buff)[i*8+7-j] = rData[j];
		MISP_MultiRead(MISP_SDRAM_ID, e_addr+(i*8+4), rData, 4);  //read 4 byte
		for(j=0;j<4;j++)	((PBYTE)line_buff)[i*8+3-j] = rData[j];
	}
#endif
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
static void FD_EnableIntegAux(void) {
	//force seamless off
	SetFC_MainLoadSizeEN(OFF);
	SetFC_AuxLoadSizeEN(OFF);

	//configure size
	MISP_RegWrite(MISP_LOCAL_ID, 0x166, FD_FRAME_HSIZE);  //set aux_mem_size_h
	MISP_RegWrite(MISP_LOCAL_ID, 0x167, FD_FRAME_VSIZE);  //set aux_mem_size_v
	MISP_RegWrite(MISP_LOCAL_ID, 0x168, FD_FRAME_HSIZE);  //aux_dst_size_h
	MISP_RegWrite(MISP_LOCAL_ID, 0x169, FD_FRAME_VSIZE);  //aux_dst_size_v
	MISP_RegWrite(MISP_LOCAL_ID, 0x16c, FD_FRAME_HSIZE);  //aux_win_size_h
	MISP_RegWrite(MISP_LOCAL_ID, 0x16d, FD_FRAME_VSIZE);  //aux_win_size_v

	//aux num frames '1', aux_skip_dis '1', aux_disp_en '1', aux_freeze '0', aux_input 'main', aux_csc 'bypass'
	MISP_RegField(MISP_LOCAL_ID, 0x144, 7, 1, 1);  //aux_skip_dis '1'
//	MISP_RegField(MISP_LOCAL_ID, 0x142, 0, 1, 0);  //aux_disp_en '0'
//	MISP_RegField(MISP_LOCAL_ID, 0x142, 1, 1, 0);  //aux_freeze '0'
	MISP_RegField(MISP_LOCAL_ID, 0x14A, 4, 4, 1);  //num_frames '1'
	MISP_RegField(MISP_LOCAL_ID, 0x14A, 0, 4, 0);  //frame_delay '0'

	//bypass AUX functions (filter/CSC)
	MISP_RegField(MISP_LOCAL_ID, 0x142, 6, 1, 1);  //aux_hfc2_bypass
	MISP_RegField(MISP_LOCAL_ID, 0x142, 5, 1, 1);  //aux_vfc_bypass
	MISP_RegField(MISP_LOCAL_ID, 0x142, 4, 1, 1);  //aux_hfc_bypass
	MISP_RegField(MISP_LOCAL_ID, 0x142, 3, 1, 1);  //afc_bypass
	MISP_RegField(MISP_LOCAL_ID, 0x180,15, 1, 1);  //aux_in_csc_bypass
	MISP_RegField(MISP_LOCAL_ID, 0x190,15, 1, 1);  //aux_out_csc_bypass

	//set decimation factor
	MISP_RegField(MISP_LOCAL_ID, 0x14B,12, 4, FD_DECI_FACTOR-1);  //aux_integral_v_deci
	MISP_RegField(MISP_LOCAL_ID, 0x14B, 8, 4, FD_DECI_FACTOR-1);  //aux_integral_h_deci

	//enable integral buffer
#if REV_BYTE_SWAP
	MISP_RegField(MISP_LOCAL_ID, 0x14B, 5, 1, 1);  //aux_integral_swap '1'
#endif
	MISP_RegField(MISP_LOCAL_ID, 0x14B, 0, 1, 1);  //aux_integral_enable '1'
	return;
}

//--------------------------------------------------------------------------------------------------------------------------
static void FD_ReqIntegUpdate(void) {
	//request update
	MISP_RegField(MISP_LOCAL_ID, 0x14B, 2, 1, 1);  //aux_integral_req '0' -> '1'
	return;
}

//--------------------------------------------------------------------------------------------------------------------------
static BOOL FD_ChkIntegUpdate(void) {
	//check update
	WORD rData=0x00;
	MISP_RegRead(MISP_LOCAL_ID, 0x14B, &rData);  //bit.3 aux_integral_done

	BOOL done=(rData&0x0008)?TRUE:FALSE;
	if(done)
		MISP_RegField(MISP_LOCAL_ID, 0x14B, 2, 1, 0);  //aux_integral_req clear

	return done;
}

//--------------------------------------------------------------------------------------------------------------------------
#if USE_OSD_BOX
#include	"..\graphic\osd.h"  //bh 2013.11.28 for OSD
void DISP_OSD_BOX(void) {
	WORD i;

	//clear previous OSD
	for(i=OSD_START; i<=OSD_LIMIT; i++)
	{
		MISPOSD_EnableSBoxBorder(&stSBOX[i], OFF);  //Box osd en/disable.
	}

	//display FD area
	BYTE OSDBoxID = OSD_START;  //bh 2013.11.28 fd test
	for(i=0; i<stFDCTRL.fd_num; i++) {
		POS_t* pPos=(POS_t*)(&(stFDCTRL.fd_pos[i]));
		MISPOSD_SetSBoxArea(&stSBOX[OSDBoxID], pPos->lx, pPos->ly, pPos->rx, pPos->ry);
		MISPOSD_SetSBoxBorderColor(&stSBOX[OSDBoxID], RGB(110,41,240));  //Blue
		MISPOSD_EnableSBoxBorder(&stSBOX[OSDBoxID], ON);  //Box osd en/disable.
		if(++OSDBoxID > OSD_LIMIT)
			break;
	}
}
#endif


//--------------------------------------------------------------------------------------------------------------------------
static void FD_PostProcess(void) {
#if USE_OSD_BOX
	DISP_OSD_BOX();
#endif
	return;
}


//--------------------------------------------------------------------------------------------------------------------------
void FD_ProcessHandler(void) {
	if(!(fFDCtrlProcCtrl&FD_ITER_START)) return;
	fFDCtrlProcCtrl&=~FD_ITER_START;

	//init FD
	if(stFDCTRL.ctrl&FDCTRL_TRIGGER_ISP) {
		stFDCTRL.ctrl &=~FDCTRL_TRIGGER_ISP;
		if(stFDCTRL.ctrl&FDCTRL_PROCESS_ON) {  //init process
			stFDCTRL.proc_step=0;
#if 1
			stFDCTRL.mem_start=(FD_AUX_MM_ADDR<<12);
			stFDCTRL.mem_count=(FD_COL_CNT_PER_LINE<<3);
#else
			stFDCTRL.mem_start=(GetDDRMAP_AUX_start_row()<<12);
			stFDCTRL.mem_count=(GetDDRMAP_AUX_col_count()<<3);
#endif
			FD_EnableIntegAux();
		}
	}
	else if(stFDCTRL.ctrl&FDCTRL_PROCESS_ON) {
		switch(stFDCTRL.proc_step) {
			case 0:  //step.0 : request
				UARTprintf("FD : request... \r\n");
				FD_ReqIntegUpdate();
				stFDCTRL.req_count=0;
				stFDCTRL.proc_step++;
				break;
			case 1:  //step.1 : check update
				//UARTprintf("FD : check... \r\n");
				if(FD_ChkIntegUpdate())
					stFDCTRL.proc_step++;  //next step
				else if(stFDCTRL.req_count>20)
					stFDCTRL.proc_step=0;  //reset
				else
					stFDCTRL.req_count++;  //wait
				break;
			default:  //step.2 : detect face
				//UARTprintf("FD : start... \r\n");
				FD_DetectFace();
				if(stFDITER.cont==0) {
					FD_PostProcess();
					UARTprintf("FD : done... \r\n\n");
					stFDCTRL.proc_step=0;
				}
				break;
		}		
	}
	return;
}

//--------------------------------------------------------------------------------------------------------------------------
void SetFD_ProcesMode(BOOL OnOff) {
	if(OnOff) {
		stFDCTRL.ctrl=FDCTRL_PROCESS_ON;
	}
	else {
		stFDCTRL.ctrl=0;
	}
	stFDCTRL.ctrl |= FDCTRL_TRIGGER_ISP;
}

//--------------------------------------------------------------------------------------------------------------------------
BOOL GetFD_ProcesMode(void) {
	if(stFDCTRL.ctrl&FDCTRL_PROCESS_ON)
		return TRUE;
	else
		return FALSE;
}

#endif  //#if defined(__USE_FD_PROCESS__)
/*  FILE_END_HERE */

