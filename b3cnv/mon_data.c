#include <b3lib.h>

uint8_t *monMacro[] = {
/*   */ "M_HOOKFANG", "M_LAND_REAVER", "M_BLACK_HOBBIT", "M_PUMA",
/*   */ "M_HOBGOBLIN", "M_SERPENT", "M_HEDGE_LIZARD", "M_DARK_GNOME",
/*   */ "M_IMP", "M_GREENCLAW", "M_BLACKCLAW", "M_BLOODWARRIOR",
/*   */ "M_VIPER", "M_WIND_DIVER", "M_WILDCAT", "M_GILA_MONSTER",
/*   */ "M_VILE_CREEPER", "M_MIASMAL_CUR", "M_WICKED_LURKER", "M_FOUL_STALKER",
/*   */ "M_BLOODFIEND", "M_GOREDROOLER", "M_DARK_PRIEST", "M_SLATHBEAST",
/*   */ "M_MAGIC_EATER_0", "M_SHADOWDRIFTER", "M_DAEMONGAR", "M_ZEPHYRLORD",
/*   */ "M_PHANTOM_0", "M_NEFAST", "M_NEFASTMASTER_0", "M_ADEPT_DAEMON",
/*   */ "M_RUBBLE_GLIDER", "M_DEATHHOUND", "M_PUTRID_ZOMBIE", "M_DARK_STALKER",
/*   */ "M_GORESUCKER", "M_ACIDDROOLER", "M_BLOOD_ACOLYTE", "M_BIG_SLATH",
/*   */ "M_MAGIC_EATER_1", "M_WING_SLASHER", "M_DAEMONSCION", "M_ZEPHYMAGE",
/*   */ "M_PHANTOM_CHAMP", "M_HUNTER_NEFAST", "M_NEFASTMASTER_1", "M_MASTER_DAEMON",
/*   */ "M_GOBLIN_GULPER", "M_DEATHLION", "M_BONE_GNAWER", "M_VENOMCLAW",
/*   */ "M_GLOOM_BARD", "M_MARROW_SUCKER", "M_BONE_ACOLYTE", "M_HUGE_SLATH",
/*   */ "M_MAGIC_EATER_2", "M_WING_DEMON", "M_CORRUPTER", "M_WIND_DAEMON",
/*   */ "M_PHANTOM_1", "M_KILLER_NEFAST", "M_NEFASTMASTER_2", "M_DAEMONGARI_LORD",
/*   */ "M_DEATH_ADDER", "M_DEATH_IMP", "M_MAD_HOWLER", "M_DEATHCLAW",
/*   */ "M_GHAST", "M_BLOOD_FEASTER", "M_SPIRIT_MAGE", "M_GRAND_SLATH",
/*   */ "M_MAGIC_EATER_3", "M_WING_ENVENOM", "M_PUTRIFIER", "M_PRIME_DAEMON",
/*   */ "M_SOULCRUSHER", "M_BLOOD_NEFAST", "M_NEFASTMASTER_3", "M_DAEMONGARI",
/*   */ "M_BRILHASTI", "M_DARK_GUARD", "M_DOLORIAN_0", "M_SHADE_DOG_0",
/*   */ "M_SHADE_GHOUL", "M_DARK_WARRIOR", "M_APE_GRUNT", "M_SHADE_WOLF",
/*   */ "M_SABLE_LICH_0", "M_IXTL_MAGE", "M_IXTL_CORSAIR_0", "M_WERE_COUGAR",
/*   */ "M_GIANT_NEWT_0", "M_SLATUS", "M_EBON_HARPY", "M_SORCERESS",
/*   */ "M_ACCURSED_ONE", "M_EBON_ZOMBIE_0", "M_HAWKSLAYER_0", "M_DOLORIAN_1",
/*   */ "M_SHADE_DOG_1", "M_IXTL_NYMPH_0", "M_APE_SOLDIER_0", "M_EBON_RANGER_0",
/*   */ "M_FAMILIAR_0", "M_SABLE_LICH_1", "M_IXTL_WIZARD_0", "M_IXTL_CORSAIR_1",
/*   */ "M_GARGOYLE_0", "M_GIANT_PYTHON_0", "M_MUD_DRAGON_0", "M_SHADE_COUGAR_0",
/*   */ "M_AVIAN_0", "M_BLACK_WIZARD_0", "M_EBON_ZOMBIE_1", "M_DOLORIAN_2",
/*   */ "M_SHADE_DOG_2", "M_IXTL_NYMPH_1", "M_APE_SOLDIER_1", "M_EBON_RANGER_1",
/*   */ "M_FAMILIAR_1", "M_SABLE_LICH_2", "M_IXTL_WIZARD_1", "M_IXTL_CORSAIR_2",
/*   */ "M_GARGOYLE_1", "M_GIANT_PYTHON_1", "M_MUD_DRAGON_1", "M_SHADE_COUGAR_1",
/*   */ "M_AVIAN_1", "M_BLACK_WIZARD_1", "M_EBON_ZOMBIE_2", "M_SHADE_PIRAHNA",
/*   */ "M_FLYING_FISH", "M_ATLANTIAN", "M_LOCHLICH", "M_JAWS",
/*   */ "M_WATER_DEMON", "M_SABLE_LICH_3", "M_GIANT_PYTHON_2", "M_EBON_SHARK",
/*   */ "M_OCTUPUS", "M_GIANT_NEWT_1", "M_WERE_EEL", "M_WATER_SPIRIT",
/*   */ "M_LAMPREY", "M_GIANT_SQUID", "M_KRAKEN", "M_DOLORIAN_3",
/*   */ "M_SHADE_DOG_3", "M_IXTL_NYMPH_2", "M_APE_SOLDIER_2", "M_EBON_RANGER_2",
/*   */ "M_FAMILIAR_2", "M_SABLE_LICH_4", "M_IXTL_WIZARD_2", "M_IXTL_CORSAIR_3",
/*   */ "M_GARGOYLE_2", "M_GIANT_PYTHON_3", "M_MUD_DRAGON_2", "M_SHADE_COUGAR_2",
/*   */ "M_AVIAN_2", "M_BLACK_WIZARD_2", "M_EBON_ZOMBIE_3", "M_HELLHOUND_0",
/*   */ "M_SHADE_LEOPARD_0", "M_IXTL_NYMPH_3", "M_APE_SOLDIER_3", "M_IXTL_DRONE_0",
/*   */ "M_FAMILIAR_3", "M_UNDEAD_AVIAN", "M_IXTL_WIZARD_3", "M_IXTL_CORSAIR_4",
/*   */ "M_SABLE_RANGER", "M_PIT_BEAST_0", "M_SWAMP_GOLEM", "M_SHADE_COUGAR_3",
/*   */ "M_AVIAN_3", "M_BLACK_GUARD", "M_APE_RANGER_0", "M_TSLOTHA_GARNATH",
/*   */ "M_HELLHOUND_1", "M_SHADE_LEOPARD_1", "M_IXTL_NYMPH_4", "M_IXTL_QUEEN",
/*   */ "M_IXTL_DRONE_1", "M_DARK_LURKER", "M_VAMPIRE_SNAKE", "M_IXTL_WIZARD_4",
/*   */ "M_EBON_CHAMPIONS", "M_SABLE_WRAITH", "M_PIT_BEAST_1", "M_TREE_FROG",
/*   */ "M_MOMMOTH", "M_EBON_ARCHMAGE", "M_APE_GENERAL", "M_APE_RANGER_1",
/*   */ "M_ICE_WARRIOR", "M_SLEET_MAGE_0", "M_GLAZE_WOLF", "M_FROST_GUARD",
/*   */ "M_YETI_0", "M_ICE_VIPER_0", "M_ARTIC_RANGER", "M_FROST_CAT",
/*   */ "M_FROSTLING", "M_BLIZZARD_DEMON_0", "M_JACK_FROST_0", "M_SLEET_MAGE_1",
/*   */ "M_FROST_TIGER", "M_KALT_MAGE_0", "M_YETI_1", "M_ICE_VIPER_1",
/*   */ "M_ICE_REAVER_0", "M_YETI_2", "M_POLAR_BEAR_0", "M_BLIZZARD_DEMON_1",
/*   */ "M_JACK_FROST_1", "M_SLEET_MAGE_2", "M_RIME_LORD_0", "M_KALT_MAGE_1",
/*   */ "M_YETI_3", "M_GLACIER_GOLEM_0", "M_ICE_REAVER_1", "M_ICE_BEAR_0",
/*   */ "M_POLAR_BEAR_1", "M_ARCTIC_MAGE_0", "M_WHITE_WIZARD", "M_GREY_WIZARD",
/*   */ "M_BLACK_WIZARD_3", "M_STOUT_RUNNER_0", "M_SLEET_MAGE_3", "M_RIME_LORD_1",
/*   */ "M_KALT_MAGE_2", "M_YETI_4", "M_GLACIER_GOLEM_1", "M_ICE_REAVER_2",
/*   */ "M_ICE_BEAR_1", "M_GLAZE_WRAITH_0", "M_ARCTIC_MAGE_1", "M_STOUT_RUNNER_1",
/*   */ "M_SLEET_MAGE_4", "M_RIME_LORD_2", "M_KALT_MAGE_3", "M_YETI_5",
/*   */ "M_GLACIER_GOLEM_2", "M_ICE_REAVER_3", "M_ICE_BEAR_2", "M_GLAZE_WRAITH_1",
/*   */ "M_ARCTIC_MAGE_2", "M_RED_DRAGON", "M_MAZE_FURY", "M_UNBELIEVER_0",
/*   */ "M_RED_EYE", "M_HERB", "M_RED_MAGE", "M_GRAVE_ROBBER_0",
/*   */ "M_FILCHER", "M_SPEED_DEMON", "M_CRAZY_EDDIE", "M_ORANGE_DRAGON",
/*   */ "M_AGENT_ORANGE", "M_UNBELIEVER_1", "M_ORANGE_OGRE", "M_MIND_MAGE_0",
/*   */ "M_SHADE_WARRIOR_0", "M_GRAVE_ROBBER_1", "M_ORANGE_WIZARD", "M_HASTE_MAGE_0",
/*   */ "M_SKEPTIC_0", "M_RAINBOW_DRAGON", "M_YELLOW_DRAGON", "M_OLD_YELLER",
/*   */ "M_BLANK_MAGE_0", "M_STONE_OGRE_0", "M_MIND_MAGE_1", "M_SHADE_WARRIOR_1",
/*   */ "M_CHAOS_FIEND_0", "M_MASK_OF_DEATH_0", "M_HASTE_MAGE_1", "M_SKEPTIC_1",
/*   */ "M_BRUTE_0", "M_GREEN_VIPER", "M_MEAN_GREEN", "M_BLANK_MAGE_1",
/*   */ "M_STONE_OGRE_1", "M_MIND_MAGE_2", "M_SHADE_WARRIOR_2", "M_CHAOS_FIEND_1",
/*   */ "M_MASK_OF_DEATH_1", "M_HASTE_MAGE_2", "M_SKEPTIC_2", "M_BRUTE_1",
/*   */ "M_CYANIS", "M_INDIGO_DRAGON", "M_WITCH_0", "M_BLANK_MAGE_2",
/*   */ "M_BLUE_GUARD", "M_MIND_MAGE_3", "M_SHADE_WARRIOR_3", "M_CHAOS_FIEND_2",
/*   */ "M_MAD_MAGE_0", "M_HASTE_MAGE_3", "M_SKEPTIC_3", "M_STRIKEFAST",
/*   */ "M_VIOLET_DRAGON", "M_WITCH_1", "M_BLANK_MAGE_3", "M_VIOLET_GUARD",
/*   */ "M_MIND_MAGE_4", "M_NIGHT_CREEPER", "M_ROMERO", "M_MAD_MAGE_1",
/*   */ "M_HASTE_MAGE_4", "M_SKEPTIC_4", "M_SHADE_WITCH", "M_PYRO",
/*   */ "M_CHAIN_VIPER", "M_SILVER_DROID", "M_IRON_DROID", "M_CLANKER",
/*   */ "M_BRASS_CLANKER", "M_HEAD_DICER", "M_GOLD_MANGLER", "M_INVULTRON",
/*   */ "M_CONVERTER_0", "M_METAL_MANIAC", "M_ALPHA_SCRAMBLER", "M_MAGIC_SYPHON_0",
/*   */ "M_RACHET_REAVER", "M_MECHMASHER_0", "M_TOASTER", "M_KINETIC_KID",
/*   */ "M_HAWKSLAYER_1", "M_SILVER_SAM", "M_WARBOT", "M_STEEL_CYBORG",
/*   */ "M_TIN_MAN", "M_BRASS_BASHER", "M_LA_MACHINE", "M_DEJUVENATOR",
/*   */ "M_MAGE_WASTER_0", "M_BLOODY_MASHER", "M_CARBON_RIPPER", "M_COLLECTOR",
/*   */ "M_MAGIC_SYPHON_1", "M_GYROGUARD", "M_MECHMASHER_1", "M_FREEZER",
/*   */ "M_GEARED_RANGER", "M_METAL_EEL", "M_SILVER_SHARK", "M_IRON_SUCKER",
/*   */ "M_ELECTRUM_EEL", "M_BRASS_JAW", "M_MERMECH", "M_WITHER_SNAKE",
/*   */ "M_MAKO_MECH", "M_SILVER_SIREN", "M_CYBORG_SQUID", "M_ELECTRIC_EEL",
/*   */ "M_MAGIC_SYPHON_2", "M_COPPER_KRAKEN", "M_MECHMASHER_2", "M_SPIRIT_SERPENT",
/*   */ "M_MK3_NEPTUNE", "M_BRONZE_BOA", "M_HEAD_BANGER", "M_STEEL_GYRO",
/*   */ "M_STEEL_GOBLIN", "M_BRASS_RENDER", "M_INFILTRATOR", "M_PETRIFIER",
/*   */ "M_MAGE_WASTER_1", "M_CONVERTER_1", "M_STEEL_SERPENT", "M_FOGGER",
/*   */ "M_MAGIC_SYPHON_3", "M_METAL_MAN", "M_MAN_MASHER", "M_OCTOMECH",
/*   */ "M_GYROHUNTER", "M_URMECH", "M_MURK_MAGE", "M_DEATH_MAGE",
/*   */ "M_BLOOD_MAGE_0", "M_KRINGLE_CUZ", "M_MAD_EYE_0", "M_EYE_CRYER",
/*   */ "M_MAD_EYE_1", "M_EYE_STALKER", "M_NOXIOUS_EYE", "M_EYE_GOTCHA",
/*   */ "M_BLUE_MAGE_0", "M_SHADOW_MAGE", "M_AUNTIE_MAGIC", "M_BLOOD_MAGE_1",
/*   */ "M_FLAME_MAGE", "M_SHADE_DRAGON", "M_SHADE_GIANT", "M_SHADE_WARRIOR_4",
/*   */ "M_PHANTOM_2", "M_VAPOR_MAGE", "M_WIND_WARRIOR", "M_BLUE_MAGE_1",
/*   */ "M_BLACK_WIZARD_4", "M_ARCH_DRAGON_0", "M_BLOOD_MAGE_2", "M_HOWLER",
/*   */ "M_LESTAT", "M_BARLOW", "M_NOSFERATU", "M_VLAD",
/*   */ "M_YORK", "M_ZOMBIE_LORD", "M_BLUE_MAGE_2", "M_BLACK_WIZARD_5",
/*   */ "M_ARCH_DRAGON_1", "M_BLOOD_MAGE_3", "M_HIGH_WIZARD", "M_FROST_MAGE",
/*   */ "M_SHOCK_MAGE", "M_UNHOLY_MAGE", "M_DARK_WITCH", "M_MASTER_SUMMONER",
/*   */ "M_ZOMBIE_MAGE", "M_BLUE_MAGE_3", "M_SCEADU", "M_NAZI_SOLDIER_0",
/*   */ "M_RUSKI_SOLDIER", "M_COMMISAR_0", "M_STORM_TROOPER", "M_RED_GUARD",
/*   */ "M_OBERLIEUTENANT", "M_RED_OFFICER", "M_SS_OFFICER", "M_GREMLIN_0",
/*   */ "M_SS_MAJOR_0", "M_RED_ARMY_HERO", "M_SS_COLONEL", "M_RED_CAPTAIN",
/*   */ "M_POLITICIAN", "M_SS_GENERAL", "M_RED_GENERAL", "M_PEASANT_0",
/*   */ "M_BUDDHIST_MONK", "M_SAMURAI", "M_SHINTO_PRIEST", "M_BANDIT",
/*   */ "M_SAMURAI_LORD", "M_AMIDA_MONK", "M_RONIN_0", "M_FOX_KANI",
/*   */ "M_KURA_OKAMI", "M_NINJA_0", "M_BEAR_WANI", "M_WARLORD",
/*   */ "M_TENGU", "M_KURA_YAMA_TSUMI", "M_CLAY_PRINCE_0", "M_SLAVE_0",
/*   */ "M_PRIESTESS", "M_SPARTAN", "M_TROJAN_PRIEST", "M_ATHENIAN",
/*   */ "M_TROJAN_0", "M_SPARTAN_ELITE", "M_TROJAN_ELITE", "M_BASILISK_0",
/*   */ "M_APOLLO_S_PRIEST", "M_TROJAN_IMMORTAL", "M_TROJAN_HERO", "M_SPARTAN_HERO",
/*   */ "M_HERA_S_PRIEST_0", "M_BASILISK_1", "M_GORGON", "M_SLAVE_1",
/*   */ "M_VESTAL_VIRGIN", "M_LEGIONNAIRE", "M_HUN", "M_GOTH_RAIDER",
/*   */ "M_HUN_WARRIOR", "M_CENTURION", "M_GLADIATOR", "M_DISCORIAN",
/*   */ "M_PRIEST", "M_VANDAL_RAIDER", "M_VISIGOTH", "M_ROMAN_GUARD_0",
/*   */ "M_JUNO_S_PRIEST", "M_VANTH", "M_SALII", "M_SAXON_PEASANT",
/*   */ "M_SAXON_WITCH", "M_YEOMAN", "M_DRUID", "M_NORMAN_0",
/*   */ "M_NORMAN_SQUIRE", "M_MERRY_MAN", "M_GOLDEN_DRAGON", "M_POOKA_0",
/*   */ "M_NORMAN_KNIGHT", "M_SLAVE_2", "M_FOX_MAIDEN", "M_PEASANT_1",
/*   */ "M_TAOIST_MONK", "M_MONGOL", "M_MONGOL_RAIDER", "M_MONGOL_PRINCE",
/*   */ "M_DRAGON_KING", "M_MONKEY_SPIRIT", "M_SHAOLIN_0", "M_DIRTGRUBBER",
/*   */ "M_FREE_TECHER_0", "M_SCAVENGER", "M_NEW_AGER_0", "M_BIKER",
/*   */ "M_MUTANT_BIKER", "M_RENEGADE", "M_DESERT_RAIDER", "M_GREMLIN_1",
/*   */ "M_LOW_LIFE", "M_DIRT_BAG", "M_NAZI_SOLDIER_1", "M_FREE_TECHER_1",
/*   */ "M_COMMISAR_1", "M_NEW_AGER_1", "M_NORMAN_1", "M_TROJAN_1",
/*   */ "M_ROMAN_GUARD_1", "M_RONIN_1", "M_POOKA_1", "M_SS_MAJOR_1",
/*   */ "M_NINJA_1", "M_SHAOLIN_1", "M_ROMAN_GUARD_2", "M_HERA_S_PRIEST_1",
/*   */ "M_MYRMIDON", "M_CLAY_PRINCE_1", "M_WERRA", "M_BLACK_SLAYER_0",
/*   */ "M_MALEFIC_CHAMP", "M_UNHOLY_TERROR", "M_BLACK_SLAYER_1", "M_BLACK_DRAGON_0",
/*   */ "M_STORM_DEMON", "M_TITAN", "M_ROCK_MAN", "M_SHADOW_DRAGON",
/*   */ "M_VORTEX_0", "M_CURSED_NUISANCE", "M_GREMLIN_2", "M_TARJAN_S_BULL_0",
/*   */ "M_TARJAN_WARRIOR_0", "M_DEATH_WARDEN_0", "M_VAMPIRE_LORD_0", "M_HELL_FIEND",
/*   */ "M_GHOUL", "M_BLACK_SLAYER_2", "M_PIT_DEMON", "M_MALEFIA_MAGE",
/*   */ "M_RAGE_WRAITH", "M_BLACK_WIZARD_6", "M_BLACK_PALADIN", "M_VORTEX_1",
/*   */ "M_ROCK_DEMON", "M_BLACK_DRAGON_1", "M_TARJAN_S_BULL_1", "M_TARJAN_WARRIOR_1",
/*   */ "M_DEATH_WARDEN_1", "M_VAMPIRE_LORD_1", "M_VORTEX_2", "M_TARJAN",
/*   */ "M_LORINI", "M_HIGH_PRIESTESS", "M_REDBEARD",
  "M_SUM_WOLF", "M_SUM_SLAYER", "M_SUM_WIND_WARRIOR", "M_SUM_WIND_OGRE",
  "M_SUM_WIND_DRAGON", "M_SUM_WIND_GIANT", "M_SUM_WIND_HERO",
  "M_SUM_FIRE_ELEMENTAL", "M_SUM_DEMON", "M_SUM_HERB", "M_SUM_GREATER_DEMON",
  "M_SUM_KRINGLE_BRO", "M_SUM_EARTH_ELEMENTAL", "M_SUM_FROST_GIANT",
  "M_SUM_MOLTEN_MAN", "M_SUM_BULLDOZER", "M_SUM_VANQUISHER",
  "M_SUM_BLAST_DRAGON", "M_SUM_ONE_EYED_ANGRA", "M_SUM_BLACK_DEATH",
  "M_SUM_FAMILIAR", "M_SUM_BLACK_SLAYER"
};

