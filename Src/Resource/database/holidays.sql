BEGIN TRANSACTION;

-- 节气
DROP TABLE IF EXISTS "solar_term";
CREATE TABLE "solar_term" (
	"name"	TEXT NOT NULL UNIQUE,
	"comment" TEXT
);

INSERT INTO "solar_term" ("name", "comment") VALUES ("冬至", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("小寒", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("大寒", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("立春", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("雨水", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("惊蛰", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("春分", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("清明", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("谷雨", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("立夏", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("小满", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("芒种", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("夏至", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("小暑", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("大暑", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("立秋", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("处暑", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("白露", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("秋分", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("寒露", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("霜降", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("立冬", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("小雪", "");
INSERT INTO "solar_term" ("name", "comment") VALUES ("大雪", "");

-- 节日
DROP TABLE IF EXISTS "holidays";
CREATE TABLE "holidays" (
	"month"	INTEGER NOT NULL,
	"day"	INTEGER NOT NULL,
	"level"	INTEGER NOT NULL DEFAULT 1,   -- 1. 基本节日 2. 其它节日
	"name"	TEXT NOT NULL,
	"comment" TEXT
);

INSERT INTO "holidays" ("month", "day", "level", "name", "comment") VALUES ("1", "1", "1", "元旦", "");
INSERT INTO "holidays" ("month", "day", "level", "name", "comment") VALUES ("3", "8", "1", "妇女节", "");
INSERT INTO "holidays" ("month", "day", "level", "name", "comment") VALUES ("5", "1", "1", "劳动节", "");
INSERT INTO "holidays" ("month", "day", "level", "name", "comment") VALUES ("6", "1", "1", "儿童节", "");
INSERT INTO "holidays" ("month", "day", "level", "name", "comment") VALUES ("8", "1", "1", "建军节", "");
INSERT INTO "holidays" ("month", "day", "level", "name", "comment") VALUES ("10", "1", "1", "国庆节", "");


/*https://baike.baidu.com/item/%E4%B8%AD%E5%9B%BD%E4%BC%A0%E7%BB%9F%E8%8A%82%E6%97%A5/396100?fromModule=disambiguation
https://baike.baidu.com/item/中国传统节日

节日名称 节日时间
春节 正月初一
元宵节（上元节）正月十五
龙抬头 二月初二
社日节（土地诞） 二月初二
花朝 南方一般2月12日，北方2月15日。但是新化叫“花烛节”为2月15日。难道祖先是从北方迁移过来的。
上巳节 三月初三
寒食节 清明节前一天
清明节 阳历4月5日前后
端午节 五月初五
天贶节 六月初六，晒衣节， 晒伏节“六月六，晒红绿。” “姑姑节”“六月六，请姑姑”。在古代还是另外一个节日，名叫天贶（赐赠的意思）节，六月六也是佛寺的一个节日，叫做翻经节，祭祀山神。
七夕节 七月初七
七月半（中元节） 七月十四/十五
中秋节 八月十五
重阳节 九月初九
寒衣节 十月初一
下元节 十月十五
冬至节 阳历12月22日前后
腊八节 腊月初八
祭灶节（小年） 腊月廿三或廿四
岁除（除夕）腊月廿九或三十*/

-- 农历节日
DROP TABLE IF EXISTS "holidays_lunnar";
CREATE TABLE "holidays_lunnar" (
	"month"	INTEGER NOT NULL,
	"day"	INTEGER NOT NULL,
	"level"	INTEGER NOT NULL DEFAULT 1,   --- 1. 基本节日 2. 其它节日
	"name"	TEXT NOT NULL UNIQUE,
	"comment" TEXT
);

INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (1,1,1,"春节","");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (1,15,1,"元宵","");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (1,15,1,"上元","");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (2,2,1,"社日","");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (2,2,1,"龙抬头","");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (2,12,1,"花朝","“花朝节”有游春扑蝶、种花挖野菜、制作花糕等节日风俗。同时，这天也称为女儿节。这天，女孩子们相聚玩耍，结交好友。未出阁的女子最爱赏红拜花神，吃花糕，行花令，祈祷自己像花一样朝气与美丽。从宋代开始，花朝节成为了民间大节，也是中国十分重要的传统节日之一。");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (3,3,1,"上巳","");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (5,5,1,"端午","");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (6,6,1,"天贶","");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (7,7,1,"七夕","");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (7,15,1,"中元","");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (8,15,1,"中秋","");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (9,9,1,"重阳","");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (10,1,1,"寒衣","");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (10,15,1,"下元","");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (12,8,1,"腊八","");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (12,24,1,"祭灶","");
INSERT INTO "holidays_lunnar" ("month","day","level","name","comment") VALUES (12,24,1,"小年","");

COMMIT;
