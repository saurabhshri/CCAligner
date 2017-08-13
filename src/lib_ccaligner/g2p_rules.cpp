/*
 * Based on grapheme to phoneme conversion in Rhubarb Lip Sync (MIT-License)
 * Author   : Daniel S Wolf (https://github.com/DanielSWolf)
 * Link     : https://github.com/DanielSWolf/rhubarb-lip-sync
 *
 * Which was originally based on Sound Change Applier (MIT-License)
 * Author   : Mark Rosenfelder
 * Link     : http://www.zompist.com/sounds.htm
 */

//
// Rules
//
// get rid of some digraphs
{ std::wregex(L"ch"), L"ç" },
{ std::wregex(L"sh"), L"$$" },
{ std::wregex(L"ph"), L"f" },
{ std::wregex(L"th"), L"+" },
{ std::wregex(L"qu"), L"kw" },
// and other spelling-level changes
{ std::wregex(L"w(r)"), L"$1" },
{ std::wregex(L"w(ho)"), L"$1" },
{ std::wregex(L"(w)h"), L"$1" },
{ std::wregex(L"(^r)h"), L"$1" },
{ std::wregex(L"(x)h"), L"$1" },
{ std::wregex(L"([aeiouäëïöüâêîôûùò@])h($)"), L"$1$2" },
{ std::wregex(L"(^e)x([aeiouäëïöüâêîôûùò@])"), L"$1gz$2" },
{ std::wregex(L"x"), L"ks" },
{ std::wregex(L"'"), L"" },
// gh is particularly variable
{ std::wregex(L"gh([aeiouäëïöüâêîôûùò@])"), L"g$1" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])a(gh)"), L"$1ä$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])e(gh)"), L"$1ë$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])i(gh)"), L"$1ï$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])o(gh)"), L"$1ö$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])u(gh)"), L"$1ü$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])â(gh)"), L"$1ä$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])ê(gh)"), L"$1ë$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])î(gh)"), L"$1ï$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])ô(gh)"), L"$1ö$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])û(gh)"), L"$1ü$2" },
{ std::wregex(L"ough(t)"), L"ò$1" },
{ std::wregex(L"augh(t)"), L"ò$1" },
{ std::wregex(L"ough"), L"ö" },
{ std::wregex(L"gh"), L"" },
// unpronounceable combinations
{ std::wregex(L"(^)g(n)"), L"$1$2" },
{ std::wregex(L"(^)k(n)"), L"$1$2" },
{ std::wregex(L"(^)m(n)"), L"$1$2" },
{ std::wregex(L"(^)p(t)"), L"$1$2" },
{ std::wregex(L"(^)p(s)"), L"$1$2" },
{ std::wregex(L"(^)t(m)"), L"$1$2" },
// medial y = i
{ std::wregex(L"(^[bcdfghjklmnpqrstvwxyzç+$ñ])y($)"), L"$1ï$2" },
{ std::wregex(L"(^[bcdfghjklmnpqrstvwxyzç+$ñ]{2})y($)"), L"$1ï$2" },
{ std::wregex(L"(^[bcdfghjklmnpqrstvwxyzç+$ñ]{3})y($)"), L"$1ï$2" },
{ std::wregex(L"ey"), L"ë" },
{ std::wregex(L"ay"), L"ä" },
{ std::wregex(L"oy"), L"öy" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])y([bcdfghjklmnpqrstvwxyzç+$ñ])"), L"$1i$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])y($)"), L"$1i$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])y(e$)"), L"$1i$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ]{2})ie($)"), L"$1ï$2" },
{ std::wregex(L"(^[bcdfghjklmnpqrstvwxyzç+$ñ])ie($)"), L"$1ï$2" },
// sSl can simplify
{ std::wregex(L"(s)t(l[aeiouäëïöüâêîôûùò@]$)"), L"$1$2" },
// affrication of t + front vowel
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñaeiouäëïöüâêîôûùò@])ci([aeiouäëïöüâêîôûùò@])"), L"$1$$$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñaeiouäëïöüâêîôûùò@])ti([aeiouäëïöüâêîôûùò@])"), L"$1$$$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñaeiouäëïöüâêîôûùò@])tu([aeiouäëïöüâêîôûùò@])"), L"$1çu$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñaeiouäëïöüâêîôûùò@])tu([rl][aeiouäëïöüâêîôûùò@])"), L"$1çu$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])si(o)"), L"$1$$$2" },
{ std::wregex(L"([aeiouäëïöüâêîôûùò@])si(o)"), L"$1j$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])s(ur)"), L"$1$$$2" },
{ std::wregex(L"([aeiouäëïöüâêîôûùò@])s(ur)"), L"$1j$2" },
{ std::wregex(L"(k)s(u[aeiouäëïöüâêîôûùò@])"), L"$1$$$2" },
{ std::wregex(L"(k)s(u[rl])"), L"$1$$$2" },
// intervocalic s
{ std::wregex(L"([eiou])s([aeiouäëïöüâêîôûùò@])"), L"$1z$2" },
// al to ol (do this before respelling)
{ std::wregex(L"a(ls)"), L"ò$1" },
{ std::wregex(L"a(lr)"), L"ò$1" },
{ std::wregex(L"a(l{2}$)"), L"ò$1" },
{ std::wregex(L"a(lm(?:[aeiouäëïöüâêîôûùò@])?$)"), L"ò$1" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])a(l[td+])"), L"$1ò$2" },
{ std::wregex(L"(^)a(l[td+])"), L"$1ò$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñaeiouäëïöüâêîôûùò@])al(k)"), L"$1ò$2" },
// soft c and g
{ std::wregex(L"c([eiêîy])"), L"s$1" },
{ std::wregex(L"c"), L"k" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñaeiouäëïöüâêîôûùò@])ge(a)"), L"$1j$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñaeiouäëïöüâêîôûùò@])ge(o)"), L"$1j$2" },
{ std::wregex(L"g([eiêîy])"), L"j$1" },
// init/final guF was there just to harden the g
{ std::wregex(L"(^)gu([eiêîy])"), L"$1g$2" },
{ std::wregex(L"gu(e$)"), L"g$1" },
// untangle reverse-written final liquids
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])re($)"), L"$1@r$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])le($)"), L"$1@l$2" },
// vowels are long medially
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])a([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@])"), L"$1ä$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])e([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@])"), L"$1ë$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])i([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@])"), L"$1ï$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])o([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@])"), L"$1ö$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])u([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@])"), L"$1ü$2" },
{ std::wregex(L"(^)a([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@])"), L"$1ä$2" }, { std::wregex(L"(^)e([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@])"), L"$1ë$2" }, { std::wregex(L"(^)i([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@])"), L"$1ï$2" }, { std::wregex(L"(^)o([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@])"), L"$1ö$2" }, { std::wregex(L"(^)u([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@])"), L"$1ü$2" },
// and short before 2 consonants or a final one
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])a([bcdfghjklmnpqrstvwxyzç+$ñ]{2})"), L"$1â$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])e([bcdfghjklmnpqrstvwxyzç+$ñ]{2})"), L"$1ê$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])i([bcdfghjklmnpqrstvwxyzç+$ñ]{2})"), L"$1î$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])o([bcdfghjklmnpqrstvwxyzç+$ñ]{2})"), L"$1ô$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])u([bcdfghjklmnpqrstvwxyzç+$ñ]{2})"), L"$1û$2" },
{ std::wregex(L"(^)a([bcdfghjklmnpqrstvwxyzç+$ñ]{2})"), L"$1â$2" }, { std::wregex(L"(^)e([bcdfghjklmnpqrstvwxyzç+$ñ]{2})"), L"$1ê$2" }, { std::wregex(L"(^)i([bcdfghjklmnpqrstvwxyzç+$ñ]{2})"), L"$1î$2" }, { std::wregex(L"(^)o([bcdfghjklmnpqrstvwxyzç+$ñ]{2})"), L"$1ô$2" }, { std::wregex(L"(^)u([bcdfghjklmnpqrstvwxyzç+$ñ]{2})"), L"$1û$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])a([bcdfghjklmnpqrstvwxyzç+$ñ]$)"), L"$1â$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])e([bcdfghjklmnpqrstvwxyzç+$ñ]$)"), L"$1ê$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])i([bcdfghjklmnpqrstvwxyzç+$ñ]$)"), L"$1î$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])o([bcdfghjklmnpqrstvwxyzç+$ñ]$)"), L"$1ô$2" }, { std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñ])u([bcdfghjklmnpqrstvwxyzç+$ñ]$)"), L"$1û$2" },
{ std::wregex(L"(^)a([bcdfghjklmnpqrstvwxyzç+$ñ]$)"), L"$1â$2" }, { std::wregex(L"(^)e([bcdfghjklmnpqrstvwxyzç+$ñ]$)"), L"$1ê$2" }, { std::wregex(L"(^)i([bcdfghjklmnpqrstvwxyzç+$ñ]$)"), L"$1î$2" }, { std::wregex(L"(^)o([bcdfghjklmnpqrstvwxyzç+$ñ]$)"), L"$1ô$2" }, { std::wregex(L"(^)u([bcdfghjklmnpqrstvwxyzç+$ñ]$)"), L"$1û$2" },
// special but general rules
{ std::wregex(L"î(nd$)"), L"ï$1" },
{ std::wregex(L"ô(s{2}$)"), L"ò$1" },
{ std::wregex(L"ô(g$)"), L"ò$1" },
{ std::wregex(L"ô(f[bcdfghjklmnpqrstvwxyzç+$ñ])"), L"ò$1" },
{ std::wregex(L"ô(l[td+])"), L"ö$1" },
{ std::wregex(L"(w)â(\\$)"), L"$1ò$2" },
{ std::wregex(L"(w)â((?:t)?ç)"), L"$1ò$2" },
{ std::wregex(L"(w)â([tdns+])"), L"$1ô$2" },
// soft gn
{ std::wregex(L"îg([mnñ]$)"), L"ï$1" },
{ std::wregex(L"îg([mnñ][bcdfghjklmnpqrstvwxyzç+$ñ])"), L"ï$1" },
{ std::wregex(L"(ei)g(n)"), L"$1$2" },
// handle ous before removing -e
{ std::wregex(L"ou(s$)"), L"@$1" },
{ std::wregex(L"ou(s[bcdfghjklmnpqrstvwxyzç+$ñ])"), L"@$1" },
// remove silent -e
{ std::wregex(L"([aeiouäëïöüâêîôûùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?(?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)e($)"), L"$1$2" },
// common suffixes that hide a silent e
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñaeiouäëïöüâêîôûùò@]{3})ë(mênt$)"), L"$1$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñaeiouäëïöüâêîôûùò@]{3})ë(nês{2}$)"), L"$1$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñaeiouäëïöüâêîôûùò@]{3})ë(li$)"), L"$1$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñaeiouäëïöüâêîôûùò@]{3})ë(fûl$)"), L"$1$2" },
// another common suffix
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñaeiouäëïöüâêîôûùò@]{3})ï(nês{2}$)"), L"$1ë$2" },
// shorten (1-char) weak penults after a long
// note: this error breaks almost as many words as it fixes...
{ std::wregex(L"([äëïöüäëïöüäëïöüùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?(?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ä([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@]$)"), L"$1â$2" }, { std::wregex(L"([äëïöüäëïöüäëïöüùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?(?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ë([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@]$)"), L"$1ê$2" }, { std::wregex(L"([äëïöüäëïöüäëïöüùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?(?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ï([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@]$)"), L"$1î$2" }, { std::wregex(L"([äëïöüäëïöüäëïöüùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?(?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ö([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@]$)"), L"$1ô$2" }, { std::wregex(L"([äëïöüäëïöüäëïöüùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?(?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ü([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@]$)"), L"$1û$2" }, { std::wregex(L"([äëïöüäëïöüäëïöüùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?(?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ä([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@]$)"), L"$1â$2" }, { std::wregex(L"([äëïöüäëïöüäëïöüùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?(?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ë([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@]$)"), L"$1ê$2" }, { std::wregex(L"([äëïöüäëïöüäëïöüùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?(?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ï([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@]$)"), L"$1î$2" }, { std::wregex(L"([äëïöüäëïöüäëïöüùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?(?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ö([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@]$)"), L"$1ô$2" }, { std::wregex(L"([äëïöüäëïöüäëïöüùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?(?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ü([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@]$)"), L"$1û$2" }, { std::wregex(L"([äëïöüäëïöüäëïöüùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?(?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ä([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@]$)"), L"$1â$2" }, { std::wregex(L"([äëïöüäëïöüäëïöüùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?(?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ë([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@]$)"), L"$1ê$2" }, { std::wregex(L"([äëïöüäëïöüäëïöüùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?(?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ï([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@]$)"), L"$1î$2" }, { std::wregex(L"([äëïöüäëïöüäëïöüùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?(?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ö([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@]$)"), L"$1ô$2" }, { std::wregex(L"([äëïöüäëïöüäëïöüùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?(?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ü([bcdfghjklmnpqrstvwxyzç+$ñ][aeiouäëïöüâêîôûùò@]$)"), L"$1û$2" },
// double vowels
{ std::wregex(L"eau"), L"ö" },
{ std::wregex(L"ai"), L"ä" },
{ std::wregex(L"au"), L"ò" },
{ std::wregex(L"âw"), L"ò" },
{ std::wregex(L"e{2}"), L"ë" },
{ std::wregex(L"ea"), L"ë" },
{ std::wregex(L"(s)ei"), L"$1ë" },
{ std::wregex(L"ei"), L"ä" },
{ std::wregex(L"eo"), L"ë@" },
{ std::wregex(L"êw"), L"ü" },
{ std::wregex(L"eu"), L"ü" },
{ std::wregex(L"ie"), L"ë" },
{ std::wregex(L"(i)[aeiouäëïöüâêîôûùò@]"), L"$1@" },
{ std::wregex(L"(^[bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)i"), L"$1ï" },
{ std::wregex(L"i(@)"), L"ë$1" },
{ std::wregex(L"oa"), L"ö" },
{ std::wregex(L"oe($)"), L"ö$1" },
{ std::wregex(L"o{2}(k)"), L"ù$1" },
{ std::wregex(L"o{2}"), L"u" },
{ std::wregex(L"oul(d$)"), L"ù$1" },
{ std::wregex(L"ou"), L"ôw" },
{ std::wregex(L"oi"), L"öy" },
{ std::wregex(L"ua"), L"ü@" },
{ std::wregex(L"ue"), L"u" },
{ std::wregex(L"ui"), L"u" },
{ std::wregex(L"ôw($)"), L"ö$1" },
// those pesky final syllabics
{ std::wregex(L"([aeiouäëïöüâêîôûùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[aeiouäëïöüâêîôûùò@])?)[aeiouäëïöüâêîôûùò@](l$)"), L"$1@$2" },
{ std::wregex(L"([aeiouäëïöüâêîôûùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ê(n$)"), L"$1@$2" },
{ std::wregex(L"([aeiouäëïöüâêîôûùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)î(n$)"), L"$1@$2" },
{ std::wregex(L"([aeiouäëïöüâêîôûùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)â(n$)"), L"$1@$2" },
{ std::wregex(L"([aeiouäëïöüâêîôûùò@][bcdfghjklmnpqrstvwxyzç+$ñ](?:[bcdfghjklmnpqrstvwxyzç+$ñ])?)ô(n$)"), L"$1@$2" },
// suffix simplifications
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñaeiouäëïöüâêîôûùò@]{3})[aâä](b@l$)"), L"$1@$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñaeiouäëïöüâêîôûùò@]l)ë(@n$)"), L"$1y$2" },
{ std::wregex(L"([bcdfghjklmnpqrstvwxyzç+$ñaeiouäëïöüâêîôûùò@]n)ë(@n$)"), L"$1y$2" },
// unpronounceable finals
{ std::wregex(L"(m)b($)"), L"$1$2" },
{ std::wregex(L"(m)n($)"), L"$1$2" },
// color the final vowels
{ std::wregex(L"a($)"), L"@$1" },
{ std::wregex(L"e($)"), L"ë$1" },
{ std::wregex(L"i($)"), L"ë$1" },
{ std::wregex(L"o($)"), L"ö$1" },
// vowels before r  V=aeiouäëïöüâêîôûùò@
{ std::wregex(L"ôw(r[bcdfghjklmnpqrstvwxyzç+$ñaeiouäëïöüâêîôûùò@])"), L"ö$1" },
{ std::wregex(L"ô(r)"), L"ö$1" },
{ std::wregex(L"ò(r)"), L"ö$1" },
{ std::wregex(L"(w)â(r[bcdfghjklmnpqrstvwxyzç+$ñ])"), L"$1ö$2" },
{ std::wregex(L"(w)â(r$)"), L"$1ö$2" },
{ std::wregex(L"ê(r{2})"), L"ä$1" },
{ std::wregex(L"ë(r[iîï][bcdfghjklmnpqrstvwxyzç+$ñ])"), L"ä$1" },
{ std::wregex(L"â(r{2})"), L"ä$1" },
{ std::wregex(L"â(r[bcdfghjklmnpqrstvwxyzç+$ñ])"), L"ô$1" },
{ std::wregex(L"â(r$)"), L"ô$1" },
{ std::wregex(L"â(r)"), L"ä$1" },
{ std::wregex(L"ê(r)"), L"@$1" },
{ std::wregex(L"î(r)"), L"@$1" },
{ std::wregex(L"û(r)"), L"@$1" },
{ std::wregex(L"ù(r)"), L"@$1" },
// handle ng
{ std::wregex(L"ng([fs$+])"), L"ñ$1" },
{ std::wregex(L"ng([bdg])"), L"ñ$1" },
{ std::wregex(L"ng([ptk])"), L"ñ$1" },
{ std::wregex(L"ng($)"), L"ñ$1" },
{ std::wregex(L"n(g)"), L"ñ$1" },
{ std::wregex(L"n(k)"), L"ñ$1" },
{ std::wregex(L"ô(ñ)"), L"ò$1" },
{ std::wregex(L"â(ñ)"), L"ä$1" },
// really a morphophonological rule, but it's cute
{ std::wregex(L"([bdg])s($)"), L"$1z$2" },
{ std::wregex(L"s(m$)"), L"z$1" },
// double consonants
{ std::wregex(L"s(s)"), L"$1" },
{ std::wregex(L"s(\\$)"), L"$1" },
{ std::wregex(L"t(t)"), L"$1" },
{ std::wregex(L"t(ç)"), L"$1" },
{ std::wregex(L"p(p)"), L"$1" },
{ std::wregex(L"k(k)"), L"$1" },
{ std::wregex(L"b(b)"), L"$1" },
{ std::wregex(L"d(d)"), L"$1" },
{ std::wregex(L"d(j)"), L"$1" },
{ std::wregex(L"g(g)"), L"$1" },
{ std::wregex(L"n(n)"), L"$1" },
{ std::wregex(L"m(m)"), L"$1" },
{ std::wregex(L"r(r)"), L"$1" },
{ std::wregex(L"l(l)"), L"$1" },
{ std::wregex(L"f(f)"), L"$1" },
{ std::wregex(L"z(z)"), L"$1" },
// There are a number of cases not covered by these rules.
// Let's add some reasonable fallback rules.
{ std::wregex(L"a"), L"â" },
{ std::wregex(L"e"), L"@" },
{ std::wregex(L"i"), L"ë" },
{ std::wregex(L"o"), L"ö" },
{ std::wregex(L"q"), L"k" },
