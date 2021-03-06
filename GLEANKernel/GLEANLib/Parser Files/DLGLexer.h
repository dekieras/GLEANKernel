#ifndef DLGLexer_h
#define DLGLexer_h
/*
 * D L G L e x e r  C l a s s  D e f i n i t i o n
 *
 * Generated from: parser.dlg
 *
 * 1989-2001 by  Will Cohen, Terence Parr, and Hank Dietz
 * Purdue University Electrical Engineering
 * DLG Version 1.33MR33
 */


#include "DLexerBase.h"

class DLGLexer : public DLGLexerBase {
public:
public:
	static const int MAX_MODE;
	static const int DfaStates;
	static const int START;
	static const int C_STYLE_COMMENT;
	static const int CPP_STYLE_COMMENT;
	static const int QUOTED_STRING;
	typedef unsigned short DfaState;

	DLGLexer(DLGInputStream *in,
		unsigned bufsize=2000)
		: DLGLexerBase(in, bufsize, 1)
	{
	;
	}
	void	  mode(int);
	ANTLRTokenType nextTokenType(void);
	void     advance(void);
protected:
	ANTLRTokenType act1();
	ANTLRTokenType act2();
	ANTLRTokenType act3();
	ANTLRTokenType act4();
	ANTLRTokenType act5();
	ANTLRTokenType act6();
	ANTLRTokenType act7();
	ANTLRTokenType act8();
	ANTLRTokenType act9();
	ANTLRTokenType act10();
	ANTLRTokenType act11();
	ANTLRTokenType act12();
	ANTLRTokenType act13();
	ANTLRTokenType act14();
	ANTLRTokenType act15();
	ANTLRTokenType act16();
	ANTLRTokenType act17();
	ANTLRTokenType act18();
	ANTLRTokenType act19();
	ANTLRTokenType act20();
	ANTLRTokenType act21();
	ANTLRTokenType act22();
	ANTLRTokenType act23();
	ANTLRTokenType act24();
	ANTLRTokenType act25();
	ANTLRTokenType act26();
	ANTLRTokenType act27();
	ANTLRTokenType act28();
	ANTLRTokenType act29();
	ANTLRTokenType act30();
	ANTLRTokenType act31();
	ANTLRTokenType act32();
	ANTLRTokenType act33();
	ANTLRTokenType act34();
	ANTLRTokenType act35();
	ANTLRTokenType act36();
	ANTLRTokenType act37();
	ANTLRTokenType act38();
	ANTLRTokenType act39();
	ANTLRTokenType act40();
	ANTLRTokenType act41();
	ANTLRTokenType act42();
	ANTLRTokenType act43();
	ANTLRTokenType act44();
	ANTLRTokenType act45();
	ANTLRTokenType act46();
	ANTLRTokenType act47();
	ANTLRTokenType act48();
	ANTLRTokenType act49();
	ANTLRTokenType act50();
	ANTLRTokenType act51();
	ANTLRTokenType act52();
	ANTLRTokenType act53();
	ANTLRTokenType act54();
	ANTLRTokenType act55();
	ANTLRTokenType act56();
	ANTLRTokenType act57();
	ANTLRTokenType act58();
	ANTLRTokenType act59();
	ANTLRTokenType act60();
	ANTLRTokenType act61();
	ANTLRTokenType act62();
	ANTLRTokenType act63();
	ANTLRTokenType act64();
	ANTLRTokenType act65();
	ANTLRTokenType act66();
	ANTLRTokenType act67();
	ANTLRTokenType act68();
	ANTLRTokenType act69();
	ANTLRTokenType act70();
	ANTLRTokenType act71();
	ANTLRTokenType act72();
	ANTLRTokenType act73();
	ANTLRTokenType act74();
	ANTLRTokenType act75();
	ANTLRTokenType act76();
	ANTLRTokenType act77();
	ANTLRTokenType act78();
	ANTLRTokenType act79();
	ANTLRTokenType act80();
	ANTLRTokenType act81();
	ANTLRTokenType act82();
	ANTLRTokenType act83();
	ANTLRTokenType act84();
	ANTLRTokenType act85();
	ANTLRTokenType act86();
	ANTLRTokenType act87();
	ANTLRTokenType act88();
	ANTLRTokenType act89();
	ANTLRTokenType act90();
	ANTLRTokenType act91();
	ANTLRTokenType act92();
	ANTLRTokenType act93();
	ANTLRTokenType act94();
	ANTLRTokenType act95();
	ANTLRTokenType act96();
	ANTLRTokenType act97();
	ANTLRTokenType act98();
	ANTLRTokenType act99();
	ANTLRTokenType act100();
	ANTLRTokenType act101();
	ANTLRTokenType act102();
	ANTLRTokenType act103();
	ANTLRTokenType act104();
	ANTLRTokenType act105();
	ANTLRTokenType act106();
	ANTLRTokenType act107();
	ANTLRTokenType act108();
	ANTLRTokenType act109();
	ANTLRTokenType act110();
	ANTLRTokenType act111();
	ANTLRTokenType act112();
	ANTLRTokenType act113();
	ANTLRTokenType act114();
	ANTLRTokenType act115();
	ANTLRTokenType act116();
	ANTLRTokenType act117();
	ANTLRTokenType act118();
	ANTLRTokenType act119();
	ANTLRTokenType act120();
	ANTLRTokenType act121();
	ANTLRTokenType act122();
	ANTLRTokenType act123();
	ANTLRTokenType act124();
	ANTLRTokenType act125();
	ANTLRTokenType act126();
	ANTLRTokenType act127();
	ANTLRTokenType act128();
	static DfaState st0[69];
	static DfaState st1[69];
	static DfaState st2[69];
	static DfaState st3[69];
	static DfaState st4[69];
	static DfaState st5[69];
	static DfaState st6[69];
	static DfaState st7[69];
	static DfaState st8[69];
	static DfaState st9[69];
	static DfaState st10[69];
	static DfaState st11[69];
	static DfaState st12[69];
	static DfaState st13[69];
	static DfaState st14[69];
	static DfaState st15[69];
	static DfaState st16[69];
	static DfaState st17[69];
	static DfaState st18[69];
	static DfaState st19[69];
	static DfaState st20[69];
	static DfaState st21[69];
	static DfaState st22[69];
	static DfaState st23[69];
	static DfaState st24[69];
	static DfaState st25[69];
	static DfaState st26[69];
	static DfaState st27[69];
	static DfaState st28[69];
	static DfaState st29[69];
	static DfaState st30[69];
	static DfaState st31[69];
	static DfaState st32[69];
	static DfaState st33[69];
	static DfaState st34[69];
	static DfaState st35[69];
	static DfaState st36[69];
	static DfaState st37[69];
	static DfaState st38[69];
	static DfaState st39[69];
	static DfaState st40[69];
	static DfaState st41[69];
	static DfaState st42[69];
	static DfaState st43[69];
	static DfaState st44[69];
	static DfaState st45[69];
	static DfaState st46[69];
	static DfaState st47[69];
	static DfaState st48[69];
	static DfaState st49[69];
	static DfaState st50[69];
	static DfaState st51[69];
	static DfaState st52[69];
	static DfaState st53[69];
	static DfaState st54[69];
	static DfaState st55[69];
	static DfaState st56[69];
	static DfaState st57[69];
	static DfaState st58[69];
	static DfaState st59[69];
	static DfaState st60[69];
	static DfaState st61[69];
	static DfaState st62[69];
	static DfaState st63[69];
	static DfaState st64[69];
	static DfaState st65[69];
	static DfaState st66[69];
	static DfaState st67[69];
	static DfaState st68[69];
	static DfaState st69[69];
	static DfaState st70[69];
	static DfaState st71[69];
	static DfaState st72[69];
	static DfaState st73[69];
	static DfaState st74[69];
	static DfaState st75[69];
	static DfaState st76[69];
	static DfaState st77[69];
	static DfaState st78[69];
	static DfaState st79[69];
	static DfaState st80[69];
	static DfaState st81[69];
	static DfaState st82[69];
	static DfaState st83[69];
	static DfaState st84[69];
	static DfaState st85[69];
	static DfaState st86[69];
	static DfaState st87[69];
	static DfaState st88[69];
	static DfaState st89[69];
	static DfaState st90[69];
	static DfaState st91[69];
	static DfaState st92[69];
	static DfaState st93[69];
	static DfaState st94[69];
	static DfaState st95[69];
	static DfaState st96[69];
	static DfaState st97[69];
	static DfaState st98[69];
	static DfaState st99[69];
	static DfaState st100[69];
	static DfaState st101[69];
	static DfaState st102[69];
	static DfaState st103[69];
	static DfaState st104[69];
	static DfaState st105[69];
	static DfaState st106[69];
	static DfaState st107[69];
	static DfaState st108[69];
	static DfaState st109[69];
	static DfaState st110[69];
	static DfaState st111[69];
	static DfaState st112[69];
	static DfaState st113[69];
	static DfaState st114[69];
	static DfaState st115[69];
	static DfaState st116[69];
	static DfaState st117[69];
	static DfaState st118[69];
	static DfaState st119[69];
	static DfaState st120[69];
	static DfaState st121[69];
	static DfaState st122[69];
	static DfaState st123[69];
	static DfaState st124[69];
	static DfaState st125[69];
	static DfaState st126[69];
	static DfaState st127[69];
	static DfaState st128[69];
	static DfaState st129[69];
	static DfaState st130[69];
	static DfaState st131[69];
	static DfaState st132[69];
	static DfaState st133[69];
	static DfaState st134[69];
	static DfaState st135[69];
	static DfaState st136[69];
	static DfaState st137[69];
	static DfaState st138[69];
	static DfaState st139[69];
	static DfaState st140[69];
	static DfaState st141[69];
	static DfaState st142[69];
	static DfaState st143[69];
	static DfaState st144[69];
	static DfaState st145[69];
	static DfaState st146[69];
	static DfaState st147[69];
	static DfaState st148[69];
	static DfaState st149[69];
	static DfaState st150[69];
	static DfaState st151[69];
	static DfaState st152[69];
	static DfaState st153[69];
	static DfaState st154[69];
	static DfaState st155[69];
	static DfaState st156[69];
	static DfaState st157[69];
	static DfaState st158[69];
	static DfaState st159[69];
	static DfaState st160[69];
	static DfaState st161[69];
	static DfaState st162[69];
	static DfaState st163[69];
	static DfaState st164[69];
	static DfaState st165[69];
	static DfaState st166[69];
	static DfaState st167[69];
	static DfaState st168[69];
	static DfaState st169[69];
	static DfaState st170[69];
	static DfaState st171[69];
	static DfaState st172[69];
	static DfaState st173[69];
	static DfaState st174[69];
	static DfaState st175[69];
	static DfaState st176[69];
	static DfaState st177[69];
	static DfaState st178[69];
	static DfaState st179[69];
	static DfaState st180[69];
	static DfaState st181[69];
	static DfaState st182[69];
	static DfaState st183[69];
	static DfaState st184[69];
	static DfaState st185[69];
	static DfaState st186[69];
	static DfaState st187[69];
	static DfaState st188[69];
	static DfaState st189[69];
	static DfaState st190[69];
	static DfaState st191[69];
	static DfaState st192[69];
	static DfaState st193[69];
	static DfaState st194[69];
	static DfaState st195[69];
	static DfaState st196[69];
	static DfaState st197[69];
	static DfaState st198[69];
	static DfaState st199[69];
	static DfaState st200[69];
	static DfaState st201[69];
	static DfaState st202[69];
	static DfaState st203[69];
	static DfaState st204[69];
	static DfaState st205[69];
	static DfaState st206[69];
	static DfaState st207[69];
	static DfaState st208[69];
	static DfaState st209[69];
	static DfaState st210[69];
	static DfaState st211[69];
	static DfaState st212[69];
	static DfaState st213[69];
	static DfaState st214[69];
	static DfaState st215[69];
	static DfaState st216[69];
	static DfaState st217[69];
	static DfaState st218[69];
	static DfaState st219[69];
	static DfaState st220[69];
	static DfaState st221[69];
	static DfaState st222[69];
	static DfaState st223[69];
	static DfaState st224[69];
	static DfaState st225[69];
	static DfaState st226[69];
	static DfaState st227[69];
	static DfaState st228[69];
	static DfaState st229[69];
	static DfaState st230[69];
	static DfaState st231[69];
	static DfaState st232[69];
	static DfaState st233[69];
	static DfaState st234[69];
	static DfaState st235[69];
	static DfaState st236[69];
	static DfaState st237[69];
	static DfaState st238[69];
	static DfaState st239[69];
	static DfaState st240[69];
	static DfaState st241[69];
	static DfaState st242[69];
	static DfaState st243[69];
	static DfaState st244[69];
	static DfaState st245[69];
	static DfaState st246[69];
	static DfaState st247[69];
	static DfaState st248[69];
	static DfaState st249[69];
	static DfaState st250[69];
	static DfaState st251[69];
	static DfaState st252[69];
	static DfaState st253[69];
	static DfaState st254[69];
	static DfaState st255[69];
	static DfaState st256[69];
	static DfaState st257[69];
	static DfaState st258[69];
	static DfaState st259[69];
	static DfaState st260[69];
	static DfaState st261[69];
	static DfaState st262[69];
	static DfaState st263[69];
	static DfaState st264[69];
	static DfaState st265[69];
	static DfaState st266[69];
	static DfaState st267[69];
	static DfaState st268[69];
	static DfaState st269[69];
	static DfaState st270[69];
	static DfaState st271[69];
	static DfaState st272[69];
	static DfaState st273[69];
	static DfaState st274[69];
	static DfaState st275[69];
	static DfaState st276[69];
	static DfaState st277[69];
	static DfaState st278[69];
	static DfaState st279[69];
	static DfaState st280[69];
	static DfaState st281[69];
	static DfaState st282[69];
	static DfaState st283[69];
	static DfaState st284[69];
	static DfaState st285[69];
	static DfaState st286[69];
	static DfaState st287[69];
	static DfaState st288[69];
	static DfaState st289[69];
	static DfaState st290[69];
	static DfaState st291[69];
	static DfaState st292[69];
	static DfaState st293[69];
	static DfaState st294[69];
	static DfaState st295[69];
	static DfaState st296[69];
	static DfaState st297[69];
	static DfaState st298[69];
	static DfaState st299[69];
	static DfaState st300[69];
	static DfaState st301[69];
	static DfaState st302[69];
	static DfaState st303[69];
	static DfaState st304[69];
	static DfaState st305[69];
	static DfaState st306[69];
	static DfaState st307[69];
	static DfaState st308[69];
	static DfaState st309[69];
	static DfaState st310[69];
	static DfaState st311[69];
	static DfaState st312[69];
	static DfaState st313[69];
	static DfaState st314[69];
	static DfaState st315[69];
	static DfaState st316[69];
	static DfaState st317[69];
	static DfaState st318[69];
	static DfaState st319[69];
	static DfaState st320[69];
	static DfaState st321[69];
	static DfaState st322[69];
	static DfaState st323[69];
	static DfaState st324[69];
	static DfaState st325[69];
	static DfaState st326[69];
	static DfaState st327[69];
	static DfaState st328[69];
	static DfaState st329[69];
	static DfaState st330[69];
	static DfaState st331[69];
	static DfaState st332[69];
	static DfaState st333[69];
	static DfaState st334[69];
	static DfaState st335[69];
	static DfaState st336[69];
	static DfaState st337[69];
	static DfaState st338[69];
	static DfaState st339[69];
	static DfaState st340[69];
	static DfaState st341[69];
	static DfaState st342[69];
	static DfaState st343[69];
	static DfaState st344[69];
	static DfaState st345[69];
	static DfaState st346[69];
	static DfaState st347[69];
	static DfaState st348[69];
	static DfaState st349[69];
	static DfaState st350[69];
	static DfaState st351[69];
	static DfaState st352[69];
	static DfaState st353[69];
	static DfaState st354[69];
	static DfaState st355[69];
	static DfaState st356[69];
	static DfaState st357[69];
	static DfaState st358[69];
	static DfaState st359[69];
	static DfaState st360[69];
	static DfaState st361[69];
	static DfaState st362[69];
	static DfaState st363[69];
	static DfaState st364[69];
	static DfaState st365[69];
	static DfaState st366[69];
	static DfaState st367[69];
	static DfaState st368[69];
	static DfaState st369[69];
	static DfaState st370[69];
	static DfaState st371[69];
	static DfaState st372[69];
	static DfaState st373[69];
	static DfaState st374[69];
	static DfaState st375[69];
	static DfaState st376[69];
	static DfaState st377[69];
	static DfaState st378[69];
	static DfaState st379[69];
	static DfaState st380[69];
	static DfaState st381[69];
	static DfaState st382[69];
	static DfaState st383[69];
	static DfaState st384[69];
	static DfaState st385[69];
	static DfaState st386[69];
	static DfaState st387[69];
	static DfaState st388[69];
	static DfaState st389[69];
	static DfaState st390[69];
	static DfaState st391[69];
	static DfaState st392[69];
	static DfaState st393[69];
	static DfaState st394[69];
	static DfaState st395[69];
	static DfaState st396[69];
	static DfaState st397[69];
	static DfaState st398[69];
	static DfaState st399[69];
	static DfaState st400[69];
	static DfaState st401[69];
	static DfaState st402[69];
	static DfaState st403[69];
	static DfaState st404[69];
	static DfaState st405[69];
	static DfaState st406[69];
	static DfaState st407[69];
	static DfaState st408[69];
	static DfaState st409[69];
	static DfaState st410[69];
	static DfaState st411[69];
	static DfaState st412[69];
	static DfaState st413[69];
	static DfaState st414[69];
	static DfaState st415[69];
	static DfaState st416[69];
	static DfaState st417[69];
	static DfaState st418[69];
	static DfaState st419[69];
	static DfaState st420[69];
	static DfaState st421[69];
	static DfaState st422[69];
	static DfaState st423[69];
	static DfaState st424[69];
	static DfaState st425[69];
	static DfaState st426[69];
	static DfaState st427[69];
	static DfaState st428[69];
	static DfaState st429[69];
	static DfaState st430[69];
	static DfaState st431[69];
	static DfaState st432[69];
	static DfaState st433[69];
	static DfaState st434[69];
	static DfaState st435[69];
	static DfaState st436[69];
	static DfaState st437[69];
	static DfaState st438[69];
	static DfaState st439[69];
	static DfaState st440[69];
	static DfaState st441[69];
	static DfaState st442[69];
	static DfaState st443[69];
	static DfaState st444[69];
	static DfaState st445[69];
	static DfaState st446[69];
	static DfaState st447[69];
	static DfaState st448[69];
	static DfaState st449[69];
	static DfaState st450[69];
	static DfaState st451[69];
	static DfaState st452[69];
	static DfaState st453[69];
	static DfaState st454[69];
	static DfaState st455[69];
	static DfaState st456[69];
	static DfaState st457[69];
	static DfaState st458[69];
	static DfaState st459[69];
	static DfaState st460[69];
	static DfaState st461[69];
	static DfaState st462[69];
	static DfaState st463[69];
	static DfaState st464[69];
	static DfaState st465[69];
	static DfaState st466[69];
	static DfaState st467[69];
	static DfaState st468[69];
	static DfaState st469[69];
	static DfaState st470[69];
	static DfaState st471[69];
	static DfaState st472[69];
	static DfaState st473[69];
	static DfaState st474[69];
	static DfaState st475[69];
	static DfaState st476[69];
	static DfaState st477[69];
	static DfaState st478[69];
	static DfaState st479[69];
	static DfaState st480[69];
	static DfaState st481[69];
	static DfaState st482[69];
	static DfaState st483[69];
	static DfaState st484[69];
	static DfaState st485[69];
	static DfaState st486[69];
	static DfaState st487[69];
	static DfaState st488[69];
	static DfaState st489[69];
	static DfaState st490[69];
	static DfaState st491[69];
	static DfaState st492[69];
	static DfaState st493[69];
	static DfaState st494[69];
	static DfaState st495[69];
	static DfaState st496[69];
	static DfaState st497[69];
	static DfaState st498[69];
	static DfaState st499[69];
	static DfaState st500[69];
	static DfaState st501[69];
	static DfaState st502[69];
	static DfaState st503[69];
	static DfaState st504[69];
	static DfaState st505[69];
	static DfaState st506[69];
	static DfaState st507[69];
	static DfaState st508[69];
	static DfaState st509[69];
	static DfaState st510[69];
	static DfaState st511[69];
	static DfaState st512[69];
	static DfaState st513[69];
	static DfaState st514[69];
	static DfaState st515[69];
	static DfaState st516[69];
	static DfaState st517[69];
	static DfaState st518[69];
	static DfaState st519[69];
	static DfaState st520[69];
	static DfaState st521[69];
	static DfaState st522[69];
	static DfaState st523[69];
	static DfaState st524[69];
	static DfaState st525[69];
	static DfaState st526[69];
	static DfaState st527[69];
	static DfaState st528[69];
	static DfaState st529[69];
	static DfaState st530[69];
	static DfaState st531[69];
	static DfaState st532[69];
	static DfaState st533[69];
	static DfaState st534[69];
	static DfaState st535[69];
	static DfaState st536[69];
	static DfaState st537[69];
	static DfaState st538[69];
	static DfaState st539[69];
	static DfaState st540[69];
	static DfaState st541[69];
	static DfaState st542[69];
	static DfaState st543[69];
	static DfaState st544[69];
	static DfaState st545[69];
	static DfaState st546[69];
	static DfaState st547[69];
	static DfaState st548[69];
	static DfaState st549[69];
	static DfaState st550[69];
	static DfaState st551[69];
	static DfaState st552[69];
	static DfaState st553[69];
	static DfaState st554[69];
	static DfaState st555[69];
	static DfaState st556[69];
	static DfaState st557[69];
	static DfaState st558[69];
	static DfaState st559[69];
	static DfaState st560[69];
	static DfaState st561[69];
	static DfaState st562[69];
	static DfaState st563[69];
	static DfaState st564[69];
	static DfaState st565[69];
	static DfaState st566[69];
	static DfaState st567[69];
	static DfaState st568[69];
	static DfaState st569[69];
	static DfaState st570[69];
	static DfaState st571[69];
	static DfaState st572[69];
	static DfaState st573[69];
	static DfaState st574[69];
	static DfaState st575[69];
	static DfaState st576[69];
	static DfaState st577[69];
	static DfaState st578[69];
	static DfaState st579[69];
	static DfaState st580[69];
	static DfaState st581[69];
	static DfaState st582[69];
	static DfaState st583[69];
	static DfaState st584[69];
	static DfaState st585[69];
	static DfaState st586[69];
	static DfaState st587[69];
	static DfaState st588[69];
	static DfaState st589[69];
	static DfaState st590[69];
	static DfaState st591[69];
	static DfaState st592[69];
	static DfaState st593[69];
	static DfaState st594[69];
	static DfaState st595[69];
	static DfaState st596[69];
	static DfaState st597[69];
	static DfaState st598[69];
	static DfaState st599[69];
	static DfaState st600[69];
	static DfaState st601[69];
	static DfaState st602[69];
	static DfaState st603[69];
	static DfaState st604[69];
	static DfaState st605[69];
	static DfaState st606[69];
	static DfaState st607[69];
	static DfaState st608[69];
	static DfaState st609[69];
	static DfaState st610[69];
	static DfaState st611[69];
	static DfaState st612[69];
	static DfaState st613[69];
	static DfaState st614[69];
	static DfaState st615[69];
	static DfaState st616[69];
	static DfaState st617[69];
	static DfaState st618[69];
	static DfaState st619[69];
	static DfaState st620[69];
	static DfaState st621[69];
	static DfaState st622[69];
	static DfaState st623[69];
	static DfaState st624[69];
	static DfaState st625[69];
	static DfaState st626[69];
	static DfaState st627[69];
	static DfaState st628[69];
	static DfaState st629[69];
	static DfaState st630[69];
	static DfaState st631[69];
	static DfaState st632[69];
	static DfaState st633[69];
	static DfaState st634[69];
	static DfaState st635[69];
	static DfaState st636[69];
	static DfaState st637[69];
	static DfaState st638[69];
	static DfaState st639[69];
	static DfaState st640[69];
	static DfaState st641[69];
	static DfaState st642[69];
	static DfaState st643[69];
	static DfaState st644[69];
	static DfaState st645[69];
	static DfaState st646[69];
	static DfaState st647[69];
	static DfaState st648[69];
	static DfaState st649[69];
	static DfaState st650[69];
	static DfaState st651[69];
	static DfaState st652[69];
	static DfaState st653[69];
	static DfaState st654[69];
	static DfaState st655[69];
	static DfaState st656[69];
	static DfaState st657[69];
	static DfaState st658[69];
	static DfaState st659[69];
	static DfaState st660[69];
	static DfaState st661[69];
	static DfaState st662[69];
	static DfaState st663[69];
	static DfaState st664[69];
	static DfaState st665[69];
	static DfaState st666[69];
	static DfaState st667[69];
	static DfaState st668[69];
	static DfaState st669[69];
	static DfaState st670[69];
	static DfaState st671[69];
	static DfaState st672[69];
	static DfaState st673[69];
	static DfaState st674[69];
	static DfaState st675[69];
	static DfaState st676[69];
	static DfaState st677[69];
	static DfaState st678[69];
	static DfaState st679[69];
	static DfaState st680[69];
	static DfaState st681[69];
	static DfaState st682[69];
	static DfaState st683[69];
	static DfaState st684[69];
	static DfaState st685[69];
	static DfaState st686[69];
	static DfaState st687[69];
	static DfaState st688[69];
	static DfaState st689[69];
	static DfaState st690[69];
	static DfaState st691[69];
	static DfaState st692[69];
	static DfaState st693[69];
	static DfaState st694[69];
	static DfaState st695[69];
	static DfaState st696[69];
	static DfaState st697[69];
	static DfaState st698[69];
	static DfaState st699[69];
	static DfaState st700[69];
	static DfaState st701[69];
	static DfaState st702[69];
	static DfaState st703[69];
	static DfaState st704[69];
	static DfaState st705[69];
	static DfaState st706[69];
	static DfaState st707[69];
	static DfaState st708[69];
	static DfaState st709[69];
	static DfaState st710[69];
	static DfaState st711[69];
	static DfaState st712[69];
	static DfaState st713[69];
	static DfaState st714[69];
	static DfaState st715[69];
	static DfaState st716[69];
	static DfaState st717[69];
	static DfaState st718[69];
	static DfaState st719[69];
	static DfaState st720[69];
	static DfaState st721[69];
	static DfaState st722[69];
	static DfaState st723[69];
	static DfaState st724[69];
	static DfaState st725[69];
	static DfaState st726[69];
	static DfaState st727[69];
	static DfaState st728[69];
	static DfaState st729[69];
	static DfaState st730[69];
	static DfaState st731[69];
	static DfaState st732[69];
	static DfaState st733[69];
	static DfaState st734[69];
	static DfaState st735[69];
	static DfaState st736[69];
	static DfaState st737[69];
	static DfaState st738[69];
	static DfaState st739[69];
	static DfaState st740[69];
	static DfaState st741[69];
	static DfaState st742[69];
	static DfaState st743[69];
	static DfaState st744[69];
	static DfaState st745[69];
	static DfaState st746[69];
	static DfaState st747[69];
	static DfaState st748[69];
	static DfaState st749[69];
	static DfaState st750[69];
	static DfaState st751[69];
	static DfaState st752[69];
	static DfaState st753[69];
	static DfaState st754[69];
	static DfaState st755[69];
	static DfaState st756[69];
	static DfaState st757[69];
	static DfaState st758[69];
	static DfaState st759[69];
	static DfaState st760[69];
	static DfaState st761[69];
	static DfaState st762[69];
	static DfaState st763[69];
	static DfaState st764[69];
	static DfaState st765[69];
	static DfaState st766[69];
	static DfaState st767[69];
	static DfaState st768[69];
	static DfaState st769[69];
	static DfaState st770[69];
	static DfaState st771[69];
	static DfaState st772[69];
	static DfaState st773[69];
	static DfaState st774[69];
	static DfaState st775[69];
	static DfaState st776[69];
	static DfaState st777[69];
	static DfaState st778[69];
	static DfaState st779[69];
	static DfaState st780[69];
	static DfaState st781[69];
	static DfaState st782[69];
	static DfaState st783[69];
	static DfaState st784[69];
	static DfaState st785[69];
	static DfaState st786[69];
	static DfaState st787[69];
	static DfaState st788[69];
	static DfaState st789[69];
	static DfaState st790[69];
	static DfaState st791[69];
	static DfaState st792[69];
	static DfaState st793[69];
	static DfaState st794[69];
	static DfaState st795[69];
	static DfaState st796[69];
	static DfaState st797[69];
	static DfaState st798[69];
	static DfaState st799[69];
	static DfaState st800[69];
	static DfaState st801[69];
	static DfaState st802[69];
	static DfaState st803[69];
	static DfaState st804[69];
	static DfaState st805[69];
	static DfaState st806[69];
	static DfaState st807[69];
	static DfaState st808[69];
	static DfaState st809[69];
	static DfaState st810[69];
	static DfaState st811[69];
	static DfaState st812[69];
	static DfaState st813[69];
	static DfaState st814[69];
	static DfaState st815[69];
	static DfaState st816[69];
	static DfaState st817[69];
	static DfaState st818[69];
	static DfaState st819[69];
	static DfaState st820[69];
	static DfaState st821[69];
	static DfaState st822[69];
	static DfaState st823[69];
	static DfaState st824[69];
	static DfaState st825[69];
	static DfaState st826[69];
	static DfaState st827[69];
	static DfaState st828[69];
	static DfaState st829[69];
	static DfaState st830[69];
	static DfaState st831[69];
	static DfaState st832[69];
	static DfaState st833[69];
	static DfaState st834[69];
	static DfaState st835[69];
	static DfaState st836[69];
	static DfaState st837[69];
	static DfaState st838[69];
	static DfaState st839[69];
	static DfaState st840[69];
	static DfaState st841[69];
	static DfaState st842[69];
	static DfaState st843[69];
	static DfaState st844[69];
	static DfaState st845[69];
	static DfaState st846[69];
	static DfaState st847[69];
	static DfaState st848[69];
	static DfaState st849[69];
	static DfaState st850[69];
	static DfaState st851[69];
	static DfaState st852[69];
	static DfaState st853[69];
	static DfaState st854[69];
	static DfaState st855[69];
	static DfaState st856[69];
	static DfaState st857[69];
	static DfaState st858[69];
	static DfaState st859[69];
	static DfaState st860[69];
	static DfaState st861[69];
	static DfaState st862[69];
	static DfaState st863[69];
	static DfaState st864[69];
	static DfaState st865[69];
	static DfaState st866[69];
	static DfaState st867[69];
	static DfaState st868[69];
	static DfaState st869[69];
	static DfaState st870[69];
	static DfaState st871[69];
	static DfaState st872[69];
	static DfaState st873[69];
	static DfaState st874[69];
	static DfaState st875[69];
	static DfaState st876[69];
	static DfaState st877[69];
	static DfaState st878[69];
	static DfaState st879[69];
	static DfaState st880[69];
	static DfaState st881[69];
	static DfaState st882[69];
	static DfaState st883[69];
	static DfaState st884[69];
	static DfaState st885[69];
	static DfaState st886[69];
	static DfaState st887[69];
	static DfaState st888[69];
	static DfaState st889[69];
	static DfaState st890[69];
	static DfaState st891[69];
	static DfaState st892[69];
	static DfaState st893[69];
	static DfaState st894[69];
	static DfaState st895[69];
	static DfaState st896[69];
	static DfaState st897[69];
	static DfaState st898[69];
	static DfaState st899[69];
	static DfaState st900[69];
	static DfaState st901[69];
	static DfaState st902[69];
	static DfaState st903[69];
	static DfaState st904[69];
	static DfaState st905[69];
	static DfaState st906[69];
	static DfaState st907[69];
	static DfaState st908[69];
	static DfaState st909[69];
	static DfaState st910[69];
	static DfaState st911[69];
	static DfaState st912[69];
	static DfaState st913[69];
	static DfaState st914[69];
	static DfaState st915[69];
	static DfaState st916[69];
	static DfaState st917[69];
	static DfaState st918[69];
	static DfaState st919[69];
	static DfaState st920[69];
	static DfaState st921[69];
	static DfaState st922[69];
	static DfaState st923[69];
	static DfaState st924[69];
	static DfaState st925[69];
	static DfaState st926[69];
	static DfaState st927[69];
	static DfaState st928[69];
	static DfaState st929[69];
	static DfaState st930[69];
	static DfaState st931[69];
	static DfaState st932[69];
	static DfaState st933[69];
	static DfaState st934[69];
	static DfaState st935[69];
	static DfaState st936[69];
	static DfaState st937[69];
	static DfaState st938[69];
	static DfaState st939[69];
	static DfaState st940[69];
	static DfaState st941[69];
	static DfaState st942[69];
	static DfaState st943[69];
	static DfaState st944[69];
	static DfaState st945[69];
	static DfaState st946[69];
	static DfaState st947[69];
	static DfaState st948[69];
	static DfaState st949[69];
	static DfaState st950[69];
	static DfaState st951[69];
	static DfaState st952[69];
	static DfaState st953[69];
	static DfaState st954[69];
	static DfaState st955[69];
	static DfaState st956[69];
	static DfaState st957[69];
	static DfaState st958[69];
	static DfaState st959[69];
	static DfaState st960[69];
	static DfaState st961[69];
	static DfaState st962[69];
	static DfaState st963[69];
	static DfaState st964[69];
	static DfaState st965[69];
	static DfaState st966[69];
	static DfaState st967[69];
	static DfaState st968[69];
	static DfaState st969[69];
	static DfaState st970[69];
	static DfaState st971[69];
	static DfaState st972[69];
	static DfaState st973[69];
	static DfaState st974[69];
	static DfaState st975[69];
	static DfaState st976[69];
	static DfaState st977[69];
	static DfaState st978[69];
	static DfaState st979[69];
	static DfaState st980[69];
	static DfaState st981[69];
	static DfaState st982[69];
	static DfaState st983[69];
	static DfaState st984[69];
	static DfaState st985[69];
	static DfaState st986[69];
	static DfaState st987[69];
	static DfaState st988[69];
	static DfaState st989[69];
	static DfaState st990[69];
	static DfaState st991[69];
	static DfaState st992[69];
	static DfaState st993[69];
	static DfaState st994[69];
	static DfaState st995[69];
	static DfaState st996[69];
	static DfaState st997[69];
	static DfaState st998[69];
	static DfaState st999[69];
	static DfaState st1000[69];
	static DfaState st1001[69];
	static DfaState st1002[69];
	static DfaState st1003[69];
	static DfaState st1004[69];
	static DfaState st1005[69];
	static DfaState st1006[69];
	static DfaState st1007[69];
	static DfaState st1008[69];
	static DfaState st1009[69];
	static DfaState st1010[69];
	static DfaState st1011[69];
	static DfaState st1012[69];
	static DfaState st1013[69];
	static DfaState st1014[69];
	static DfaState st1015[69];
	static DfaState st1016[69];
	static DfaState st1017[69];
	static DfaState st1018[69];
	static DfaState st1019[69];
	static DfaState st1020[69];
	static DfaState st1021[69];
	static DfaState st1022[69];
	static DfaState st1023[69];
	static DfaState st1024[69];
	static DfaState st1025[69];
	static DfaState st1026[69];
	static DfaState st1027[69];
	static DfaState st1028[69];
	static DfaState st1029[7];
	static DfaState st1030[7];
	static DfaState st1031[7];
	static DfaState st1032[7];
	static DfaState st1033[7];
	static DfaState st1034[7];
	static DfaState st1035[7];
	static DfaState st1036[5];
	static DfaState st1037[5];
	static DfaState st1038[5];
	static DfaState st1039[5];
	static DfaState st1040[5];
	static DfaState st1041[6];
	static DfaState st1042[6];
	static DfaState st1043[6];
	static DfaState st1044[6];
	static DfaState st1045[6];
	static DfaState st1046[6];
	static DfaState st1047[6];
	static DfaState *dfa[1048];
	static DfaState dfa_base[];
	static unsigned char *b_class_no[];
	static DfaState accepts[1049];
	static DLGChar alternatives[1049];
	static ANTLRTokenType (DLGLexer::*actions[129])();
	static unsigned char shift0[257];
	static unsigned char shift1[257];
	static unsigned char shift2[257];
	static unsigned char shift3[257];
	int ZZSHIFT(int c) { return b_class_no[automaton][1+c]; }
//
// 133MR1 Deprecated feature to allow inclusion of user-defined code in DLG class header
//
#ifdef DLGLexerIncludeFile
#include DLGLexerIncludeFile
#endif
};
typedef ANTLRTokenType (DLGLexer::*PtrDLGLexerMemberFunc)();
#endif
