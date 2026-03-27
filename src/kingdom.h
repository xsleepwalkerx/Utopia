#include <stdlib.h>
#include <limits.h>
#if !defined(WIN32)
#include <sys/cdefs.h>
#endif
#include <sys/time.h>  

void do_kingdomlist             args( (CHAR_DATA *ch, char *argument) );
void do_induct                  args( (CHAR_DATA *ch, char *argument) );
void do_outcast                 args( (CHAR_DATA *ch, char *argument) );
void do_promoterank             args( (CHAR_DATA *ch, char *argument) );
void do_seeking                 args( (CHAR_DATA *ch, char *argument) );
void do_viewkingdom             args( (CHAR_DATA *ch, char *argument) );
void do_krecall                 args( (CHAR_DATA *ch, char *argument) );
void do_viewdungeon             args( (CHAR_DATA *ch, char *argument) );
void do_imprison                args( (CHAR_DATA *ch, char *argument) );
void do_dungeonrelease  args( (CHAR_DATA *ch, char *argument) );
void do_depositqps              args( (CHAR_DATA *ch, char *argument) );
void do_defect                  args( (CHAR_DATA *ch, char *argument) );
void do_renounce                args( (CHAR_DATA *ch, char *argument) );
void do_abdicate                args( (CHAR_DATA *ch, char *argument) );
void do_kingdomedit             args( (CHAR_DATA *ch, char *argument) );
void    load_kingdoms           args( (void) );
void    save_kingdoms           args( (void) );
void    initialize_kingdoms     args( (void) );

typedef struct  kingdom_data            KINGDOM_DATA;
typedef struct  room_own_data           ROOM_OWN_DATA;
#define IS_KFLAG(kingdom,bit)   IS_SET((kingdom).flags,bit)
#define KINGDOM_UNUSED          0x00000001
#define OWN_FULL        0x00000001
#define OWN_DISPUTED    0x00000002
#define OWN_DUAL        0x00000004
#define OWN_CLAIM       0x00000008
struct room_own_data
{
    int                         vnum;
    ROOM_INDEX_DATA *           pRoom;
    int                         type;
    int                         flags;
    ROOM_OWN_DATA *             next;
};
typedef struct  war_data                WAR_DATA;
void load_war   args( (void) );
void save_war   args( (void) );
struct war_data
{
        int one;
        int two;
};
#define MAX_WAR                    45 /* MAX_KINGDOM-1 * MAX_KINGDOM /2 total combos*/
WAR_DATA war_table[MAX_WAR];
#define KINGDOM_OBJ_VNUM_START                          90000
#define MAX_KINGDOM 10
#define IS_CURSED(whome)                                        (IS_AFFECTED(whome, AFF_CURSE))
struct kingdom_data
{
        OBJ_DATA *object;               //Object (Currently Unused)
        int num;                                //Number, what number is it
        char *name;                             //Name, what name is on clanlist
        char *who_name;                 //Who_Name, what name is on who
        char *leader_name;              //Leader,  Who is the leader (spelling important)
        char *flag;                     //Kingdom flag/banner
        int flags;                      //Kingdom Bits
        int pk;                                 //Pkills, Combined pkills of its members
        int pd;                                 //Pdeaths, Combined pdeaths of its members
        int recall;                             //Recall, What vnum the krecall is
        int altar;                              //Altar, What vnum the kaltar is (currently unused)
                                                        //NEW
        int mpk;                                //MobPkills, How many invaders have lost to castle guards.
        int qps;                                //QuestPoints, how many quest points are stored here.
        int clan_sn;                    //Clan Spell Number, what is the slot number of the kingdom spell.
        int members;                    //Members, how many individuals belong to the kingdom.
        int dungeon;                    //Dungeon -> room vnum.
        char *rank1m;                   //Rank 1 Male Name 16 character long string.    (Peasant)
        char *rank2m;                   //Rank 2 Male Name 16 character long string.    (Knight)
        char *rank3m;                   //Rank 3 Male Name 16 character long string.    (Baron)
        char *rank4m;                   //Rank 4 Male Name 16 character long string.    (Prince)
        char *rank5m;                   //Rank 5 Male Name 16 character long string.    (King)
        char *rank1f;                   //Rank 1 Female Name 16 character long string.  (Peasant)
        char *rank2f;                   //Rank 2 Female Name 16 character long string.  (Knight)
        char *rank3f;                   //Rank 3 Female Name 16 character long string.  (Baroness)
        char *rank4f;                   //Rank 4 Female Name 16 character long string.  (Princess)
        char *rank5f;                   //Rank 5 Female Name 16 character long string.  (Queen)
                                                        //CLANREQS
        int r_stats             [3];    //Hp/Mana/Move requirements, like 15000,15000,15000
        int r_stancenum [2];
 int r_stancemin [2];
        int r_weaponnum [2];
        int r_weaponmin [2];

        ROOM_OWN_DATA   *claim;
        int             ikr[MAX_KINGDOM];
};

extern KINGDOM_DATA king_table[MAX_KINGDOM];
/*
 * Kingdom Commands
 */
 DECLARE_DO_FUN( do_seeking      );
DECLARE_DO_FUN( do_kingdomlist  );
DECLARE_DO_FUN( do_warlist  );
//DECLARE_DO_FUN( do_endwar  );
DECLARE_DO_FUN( do_eliminate  );
DECLARE_DO_FUN( do_startwar  );
DECLARE_DO_FUN( do_game );
DECLARE_DO_FUN( do_slots);
DECLARE_DO_FUN( do_high_dice);
//DECLARE_DO_FUN( do_war );
DECLARE_DO_FUN( do_declarewar  );
DECLARE_DO_FUN( do_declarepeace  );
DECLARE_DO_FUN( do_viewkingdom  );
DECLARE_DO_FUN( do_defect       );
DECLARE_DO_FUN( do_depositqps   );
DECLARE_DO_FUN( do_renounce     );
DECLARE_DO_FUN( do_krecall      );
DECLARE_DO_FUN( do_viewdungeon  );
DECLARE_DO_FUN( do_abdicate     );
DECLARE_DO_FUN( do_kingdomedit  );
DECLARE_DO_FUN( do_promoterank  );
DECLARE_DO_FUN( do_dungeonrelease);
DECLARE_DO_FUN( do_kload        );
DECLARE_DO_FUN( do_ksave        );
DECLARE_DO_FUN( do_imprison     );
DECLARE_DO_FUN( do_ktalk        );
DECLARE_DO_FUN( do_kchan        );
DECLARE_DO_FUN(	do_clannum	);
DECLARE_DO_FUN( do_clanset	);
DECLARE_DO_FUN( do_clanlist      );
DECLARE_DO_FUN( do_clantable      );
DECLARE_DO_FUN( do_classset    );
DECLARE_DO_FUN( do_classestable      );
DECLARE_DO_FUN( do_kingnum      );
DECLARE_DO_FUN( do_kingset      );
