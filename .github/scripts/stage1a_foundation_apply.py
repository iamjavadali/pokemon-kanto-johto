from pathlib import Path
import re

ROOT = Path('.')


def read(path):
    return (ROOT / path).read_text()


def write(path, text):
    p = ROOT / path
    p.parent.mkdir(parents=True, exist_ok=True)
    p.write_text(text)


def replace_exact(path, old, new, expected=1):
    text = read(path)
    count = text.count(old)
    if count != expected:
        raise SystemExit(f'{path}: expected {expected} occurrence(s), found {count}: {old[:100]!r}')
    write(path, text.replace(old, new))


def regex_replace(path, pattern, repl, expected=1, flags=0):
    text = read(path)
    new_text, count = re.subn(pattern, repl, text, flags=flags)
    if count != expected:
        raise SystemExit(f'{path}: expected {expected} regex replacement(s), found {count}: {pattern!r}')
    write(path, new_text)


# 1. Restore ordinary Pikachu to the expansion's modern stat configuration.
replace_exact(
    'src/data/pokemon/species_info/gen_1_families.h',
    '''    [SPECIES_PIKACHU] =\n    {\n        .baseHP        = 35,\n        .baseAttack    = 55,\n        .baseDefense   = 30,\n        .baseSpeed     = 90,\n        .baseSpAttack  = 50,\n        .baseSpDefense = 50,''',
    '''    [SPECIES_PIKACHU] =\n    {\n        .baseHP        = 35,\n        .baseAttack    = 55,\n        .baseDefense   = PIKACHU_DEFENSE,\n        .baseSpeed     = 90,\n        .baseSpAttack  = 50,\n        .baseSpDefense = PIKACHU_SP_DEF,'''
)

# 2. Reserve persistent project state without changing save structs.
replace_exact(
    'include/constants/vars_frlg.h',
    '#define VAR_0x4029                         0x4029',
    '#define VAR_KANTO_JOHTO_CAMPAIGN           0x4029 // 0=Yellow Kanto; see constants/kanto_johto.h'
)
replace_exact(
    'include/constants/flags_frlg.h',
    '#define FLAG_0x0B0                         0x0B0',
    '#define FLAG_PARTNER_PIKACHU_FOLLOWING     0x0B0 // Set only when Pikachu exits its Ball after the Oak Lab rival battle'
)

write('include/constants/kanto_johto.h', '''#ifndef GUARD_CONSTANTS_KANTO_JOHTO_H\n#define GUARD_CONSTANTS_KANTO_JOHTO_H\n\n// Persistent campaign phases stored in VAR_KANTO_JOHTO_CAMPAIGN.\n// The default zero value intentionally means the initial Yellow Kanto campaign.\nenum KantoJohtoCampaignPhase\n{\n    KANTO_JOHTO_CAMPAIGN_YELLOW_KANTO = 0,\n    KANTO_JOHTO_CAMPAIGN_SEVII,\n    KANTO_JOHTO_CAMPAIGN_TIME_SKIP,\n    KANTO_JOHTO_CAMPAIGN_JOHTO,\n    KANTO_JOHTO_CAMPAIGN_KANTO_RETURN,\n    KANTO_JOHTO_CAMPAIGN_ENDGAME,\n};\n\n#define KANTO_JOHTO_GEN2_UNLOCK_PHASE KANTO_JOHTO_CAMPAIGN_JOHTO\n#define PARTNER_PIKACHU_JOHTO_MIN_CHALLENGE_LEVEL 30\n\n#endif // GUARD_CONSTANTS_KANTO_JOHTO_H\n''')

# 3. Oak gives the existing expansion Partner/Starter Pikachu form, not ordinary Pikachu.
replace_exact(
    'data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc',
    '\tsetvar PLAYER_STARTER_SPECIES, SPECIES_PIKACHU\n',
    '\tsetvar PLAYER_STARTER_SPECIES, SPECIES_PIKACHU_STARTER\n'
)
replace_exact(
    'data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc',
    '''\tsetflag FLAG_BEAT_RIVAL_IN_OAKS_LAB\n\tcallnative UpdateFollowingPokemon\n\tdelay 30\n\tplaymoncry SPECIES_PIKACHU, CRY_MODE_NORMAL''',
    '''\tsetflag FLAG_BEAT_RIVAL_IN_OAKS_LAB\n\tsetflag FLAG_PARTNER_PIKACHU_FOLLOWING\n\tcallnative UpdateFollowingPokemon\n\tdelay 30\n\tplaymoncry SPECIES_PIKACHU_STARTER, CRY_MODE_NORMAL'''
)

# 4. Follower selection becomes Partner-aware and independent of party slot.
path = 'src/event_object_movement.c'
text = read(path)
prototype_marker = 'static enum Species GetUnownSpecies(struct Pokemon *mon);\n'
if text.count(prototype_marker) != 1:
    raise SystemExit('event_object_movement.c: GetUnownSpecies prototype marker drifted')
text = text.replace(prototype_marker, prototype_marker + 'static struct Pokemon *GetPartnerAwareFollowingMon(void);\n', 1)
implementation = '''\nstatic struct Pokemon *GetPartnerAwareFollowingMon(void)\n{\n    u32 i;\n    struct Pokemon *partner = NULL;\n\n    for (i = 0; i < PARTY_SIZE; i++)\n    {\n        if (GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES) == SPECIES_PIKACHU_STARTER)\n        {\n            partner = &gParties[B_TRAINER_PLAYER][i];\n            break;\n        }\n    }\n\n    // The Partner exists before the Yellow story permits it to follow.\n    // Suppress all automatic follower spawning until the Oak Lab event sets this flag.\n    if (partner != NULL)\n        return FlagGet(FLAG_PARTNER_PIKACHU_FOLLOWING) ? partner : NULL;\n\n    return GetFirstLiveMon();\n}\n\n'''
insert_marker = 'const u8 gReflectionEffectPaletteMap[16] = {'
if text.count(insert_marker) != 1:
    raise SystemExit('event_object_movement.c: reflection palette marker drifted')
text = text.replace(insert_marker, implementation + insert_marker, 1)
old_info = 'return GetMonInfo(GetFirstLiveMon(), species, shiny, female);'
if text.count(old_info) != 1:
    raise SystemExit(f'event_object_movement.c: expected one follower info resolver, found {text.count(old_info)}')
text = text.replace(old_info, 'return GetMonInfo(GetPartnerAwareFollowingMon(), species, shiny, female);', 1)
old_action = 'struct Pokemon *mon = GetFirstLiveMon();'
if text.count(old_action) != 1:
    raise SystemExit(f'event_object_movement.c: expected one follower action resolver, found {text.count(old_action)}')
text = text.replace(old_action, 'struct Pokemon *mon = GetPartnerAwareFollowingMon();', 1)
write(path, text)

# 5. Prevent Partner Pikachu from being deposited, moved into PC storage, or released.
path = 'src/pokemon_storage_system.c'
text = read(path)
text = text.replace('    MSG_LAST_POKE,\n    MSG_PARTY_FULL,', '    MSG_LAST_POKE,\n    MSG_PARTNER_PIKACHU_STAYS,\n    MSG_PARTY_FULL,', 1)
text = text.replace('    MSTATE_ERROR_LAST_PARTY_MON,\n    MSTATE_ERROR_HAS_MAIL,', '    MSTATE_ERROR_LAST_PARTY_MON,\n    MSTATE_ERROR_PARTNER_PIKACHU,\n    MSTATE_ERROR_HAS_MAIL,', 1)
text = text.replace('static bool8 IsRemovingLastPartyMon(void);\n', 'static bool8 IsPartnerPikachuAtPartyCursor(void);\nstatic bool8 IsRemovingLastPartyMon(void);\n', 1)
msg_old = '    [MSG_LAST_POKE]            = {COMPOUND_STRING("That\\'s your last POKéMON!"),  MSG_VAR_NONE},\n'
if msg_old not in text:
    # The source uses a literal apostrophe; keep an explicit fallback for guard clarity.
    msg_old = '    [MSG_LAST_POKE]            = {COMPOUND_STRING("That\'s your last POKéMON!"),  MSG_VAR_NONE},\n'
if text.count(msg_old) != 1:
    raise SystemExit('pokemon_storage_system.c: MSG_LAST_POKE table entry drifted')
text = text.replace(msg_old, msg_old + '    [MSG_PARTNER_PIKACHU_STAYS] = {COMPOUND_STRING("PIKACHU won\\'t leave your side."), MSG_VAR_NONE},\n', 1)

# Main input cases.
text, n = re.subn(
    r'(        case INPUT_DEPOSIT:\n)            if \(!IsRemovingLastPartyMon\(\)\)',
    r'\1            if (IsPartnerPikachuAtPartyCursor())\n            {\n                sStorage->state = MSTATE_ERROR_PARTNER_PIKACHU;\n            }\n            else if (!IsRemovingLastPartyMon())',
    text,
    count=1,
)
if n != 1:
    raise SystemExit('pokemon_storage_system.c: INPUT_DEPOSIT guard drifted')
text, n = re.subn(
    r'(        case INPUT_MOVE_MON:\n)            if \(IsRemovingLastPartyMon\(\)\)',
    r'\1            if (IsPartnerPikachuAtPartyCursor())\n            {\n                sStorage->state = MSTATE_ERROR_PARTNER_PIKACHU;\n            }\n            else if (IsRemovingLastPartyMon())',
    text,
    count=1,
)
if n != 1:
    raise SystemExit('pokemon_storage_system.c: INPUT_MOVE_MON guard drifted')
text, n = re.subn(
    r'(        case MENU_RELEASE:\n)            if \(IsRemovingLastPartyMon\(\)\)',
    r'\1            if (IsPartnerPikachuAtPartyCursor())\n            {\n                sStorage->state = MSTATE_ERROR_PARTNER_PIKACHU;\n            }\n            else if (IsRemovingLastPartyMon())',
    text,
    count=1,
)
if n != 1:
    raise SystemExit('pokemon_storage_system.c: MENU_RELEASE guard drifted')

state_old = '''    case MSTATE_ERROR_LAST_PARTY_MON:\n        PlaySE(SE_FAILURE);\n        PrintMessage(MSG_LAST_POKE);\n        sStorage->state = MSTATE_WAIT_ERROR_MSG;\n        break;\n    case MSTATE_ERROR_HAS_MAIL:'''
state_new = '''    case MSTATE_ERROR_LAST_PARTY_MON:\n        PlaySE(SE_FAILURE);\n        PrintMessage(MSG_LAST_POKE);\n        sStorage->state = MSTATE_WAIT_ERROR_MSG;\n        break;\n    case MSTATE_ERROR_PARTNER_PIKACHU:\n        PlaySE(SE_FAILURE);\n        PrintMessage(MSG_PARTNER_PIKACHU_STAYS);\n        sStorage->state = MSTATE_WAIT_ERROR_MSG;\n        break;\n    case MSTATE_ERROR_HAS_MAIL:'''
if text.count(state_old) != 1:
    raise SystemExit('pokemon_storage_system.c: error-state switch drifted')
text = text.replace(state_old, state_new, 1)

helper_marker = '''static bool8 IsRemovingLastPartyMon(void)\n{'''
helper_impl = '''static bool8 IsPartnerPikachuAtPartyCursor(void)\n{\n    return sCursorArea == CURSOR_AREA_IN_PARTY\n        && !sIsMonBeingMoved\n        && GetMonData(&gParties[B_TRAINER_PLAYER][sCursorPosition], MON_DATA_SPECIES) == SPECIES_PIKACHU_STARTER;\n}\n\nstatic bool8 IsRemovingLastPartyMon(void)\n{'''
if text.count(helper_marker) != 1:
    raise SystemExit('pokemon_storage_system.c: IsRemovingLastPartyMon definition drifted')
text = text.replace(helper_marker, helper_impl, 1)
write(path, text)

# 6. Daycare scripts reject the Partner while leaving normal Pikachu untouched.
path = 'data/scripts/day_care_frlg.inc'
text = read(path)
needle = '\tspecialvar VAR_0x8005, GetSelectedMonNicknameAndSpecies\n\tmsgbox Route5_PokemonDayCare_Text_LookAfterMonForAWhile'
replacement = '\tspecialvar VAR_0x8005, GetSelectedMonNicknameAndSpecies\n\tgoto_if_eq VAR_0x8005, SPECIES_PIKACHU_STARTER, Route5_PokemonDayCare_EventScript_PartnerPikachuStays\n\tmsgbox Route5_PokemonDayCare_Text_LookAfterMonForAWhile'
if text.count(needle) != 1:
    raise SystemExit('day_care_frlg.inc: Route 5 selected-mon hook drifted')
text = text.replace(needle, replacement, 1)
insert = '''\nRoute5_PokemonDayCare_EventScript_PartnerPikachuStays::\n\tmsgbox Route5_PokemonDayCare_Text_PartnerPikachuStays\n\tgoto Route5_PokemonDayCare_EventScript_CloseMoneyBox\n\tend\n\nRoute5_PokemonDayCare_Text_PartnerPikachuStays::\n\t.string "PIKACHU refuses to stay\\nbehind.$"\n'''
text += insert
write(path, text)

path = 'data/maps/FourIsland_PokemonDayCare_Frlg/scripts.inc'
text = read(path)
needle = '\tspecialvar VAR_0x8005, GetSelectedMonNicknameAndSpecies\n\twaitse\n\tplaymoncry VAR_0x8005, CRY_MODE_NORMAL'
replacement = '\tspecialvar VAR_0x8005, GetSelectedMonNicknameAndSpecies\n\tgoto_if_eq VAR_0x8005, SPECIES_PIKACHU_STARTER, FourIsland_PokemonDayCare_PartnerPikachuStays\n\twaitse\n\tplaymoncry VAR_0x8005, CRY_MODE_NORMAL'
if text.count(needle) != 1:
    raise SystemExit('FourIsland daycare: selected-mon hook drifted')
text = text.replace(needle, replacement, 1)
text += '''\nFourIsland_PokemonDayCare_PartnerPikachuStays::\n\tmsgbox FourIsland_PokemonDayCare_Text_PartnerPikachuStays\n\trelease\n\tend\n\nFourIsland_PokemonDayCare_Text_PartnerPikachuStays::\n\t.string "PIKACHU refuses to stay\\nbehind.$"\n'''
write(path, text)

# 7. Replace stale project rules with the approved campaign/Partner architecture.
path = 'docs/projects/KANTO_JOHTO_MASTER_RULES.md'
text = read(path)
text = text.replace(
    '- Kanto source inspiration: FireRed/LeafGreen plus selected Pokemon Yellow/anime-style events.\n- The opening starter-acquisition sequence is a special case: the actual Pokémon Yellow game is the canonical source of truth and its ordered story beats must not be replaced by anime recollections or approximations.\n- Johto source inspiration: Gold/Silver/Crystal, with selective HeartGold/SoulSilver ideas used as design reference only.',
    '- Kanto story authority: Pokémon Yellow. FireRed/LeafGreen supplies the GBA-era presentation, Kanto map foundation, UI conventions, and the Sevii campaign; it does not replace Yellow-specific Kanto story events.\n- Johto story authority: Pokémon Crystal, with Gold/Silver supplying compatible version-split or omitted material. HeartGold/SoulSilver may be used only as selective presentation/QoL reference.\n- Emerald story is not part of the Kanto/Johto campaign. Selected Emerald systems or official-event concepts such as Faraway Island and Battle Frontier may be adapted where they strengthen the unified game.'
)
old_opening = '''## Opening and starter philosophy\n- Pikachu is the player's original partner/starter through the canonical Pokémon Yellow starter-acquisition sequence.\n- Pikachu remains a normal usable Pokemon: trainable, boxable, breedable when breeding is available, and evolvable into Raichu by player choice.\n- Bulbasaur, Charmander, and Squirtle are acquired later through story quests inspired by their memorable anime acquisition stories rather than handed out as a starter selection.\n- Chikorita, Cyndaquil, and Totodile are likewise acquired through Johto story quests rather than a one-of-three permanent choice.\n- Story-acquired Pokemon remain normal Pokemon after acquisition. The anime may inspire how they join the player but must not force their later evolution, departure, moves, party position, or usage.\n\n## Starter Pikachu progression contract\n- Starter Pikachu begins at level 5 and initially knows only `Thunder Shock` and `Growl`, matching Pokémon Yellow.\n- Regular Pikachu uses the Pokémon Yellow species stat profile: HP 35, Attack 55, Defense 30, Speed 90, and Special-equivalent 50. In the Gen III split-stat engine, both Special Attack and Special Defense use 50 for this Yellow-faithful profile.\n- Pikachu uses the Medium Fast experience-growth curve.\n- Pikachu's level-up learnset must preserve the Pokémon Yellow schedule: Tail Whip at 6, Thunder Wave at 8, Quick Attack at 11, Double Team at 15, Slam at 20, Thunderbolt at 26, Agility at 33, Thunder at 41, and Light Screen at 50.\n- Do not add an artificial experience penalty, hidden stat handicap, or starter-only weakening mechanic.\n- Selective modern battle/QoL mechanics may remain where the wider project deliberately uses them, but they must not accidentally replace or weaken the Yellow starter progression contract above.\n'''
new_opening = '''## Opening and starter philosophy\n- Pikachu is the player's original Partner through the canonical Pokémon Yellow starter-acquisition sequence.\n- The owned story Partner uses the expansion's persistent `SPECIES_PIKACHU_STARTER` identity while still presenting as Pokédex #025 Pikachu. Ordinary wild Pikachu remain ordinary Pikachu.\n- Partner Pikachu keeps Yellow's story identity: it refuses evolution, refuses to be traded away, refuses ordinary PC/daycare separation, and begins following only at Yellow's post-rival Oak Lab story beat.\n- Partner Pikachu uses modern mechanics rather than Generation I limitations: Natures, IVs/EVs, Abilities, physical/special split, modern move behavior, modern battle UI, and the expansion's stronger Partner Pikachu stat profile.\n- Ordinary Pikachu retain the expansion's modern regular-Pikachu stats and can evolve, be boxed, use daycare, and otherwise behave normally.\n- Bulbasaur, Charmander, and Squirtle must use their real Pokémon Yellow gift events: Melanie/friendship in Cerulean, Damian on Route 24, and Officer Jenny after the Thunder Badge in Vermilion.\n- On entering Johto, Professor Elm offers Chikorita, Cyndaquil, or Totodile as the protagonist's Johto research partner; the Johto rival steals another starter as in Crystal.\n\n## Partner Pikachu progression contract\n- Partner Pikachu begins at level 5 with Thunder Shock and Growl in the Yellow opening, but future progression uses approved modernized move/mechanics behavior rather than freezing the species to Generation I statistics.\n- The current engine's built-in Partner Pikachu profile is the Stage 1A baseline: 45 HP / 80 Attack / 50 Defense / 120 Speed / 75 Sp. Atk / 60 Sp. Def, Medium Fast growth, modern Nature/IV/EV/Ability mechanics, no evolution entry, and `cannotBeTraded`.\n- Exact long-term balance may be tuned against the Kanto difficulty curve, but any tuning must remain Partner-only and must never globally weaken or boost ordinary Pikachu.\n- During Johto, Partner Pikachu will refuse challenges rated below level 30. This future rule is opponent-directed arrogance, not loss of respect for the protagonist, and must use deterministic eligibility rather than random traded-Pokémon disobedience.\n'''
if old_opening not in text:
    raise SystemExit('master rules: stale opening/progression block drifted')
text = text.replace(old_opening, new_opening, 1)
text = text.replace(
    '- Party slot #1 should be able to act as the player\'s overworld follower once the general follower system is enabled.\n- Pikachu can have special story reactions but is not permanently forced as the follower after the opening.',
    '- The general follower engine remains available, but once the Yellow Oak Lab follower beat occurs, Partner Pikachu is the persistent protagonist follower regardless of its party slot.\n- Party order and battle lead are independent from the Partner follower identity so the Johto starter can lead while Pikachu continues following.'
)
text = text.replace(
    '- Planned macro structure: Kanto -> Kanto League -> Sevii transition/expansion -> Johto -> Johto/combined League progression -> late-game resolution -> legendary quests/Mt. Silver/endgame.',
    '- Planned macro structure: Yellow Kanto -> Kanto League -> FireRed Sevii -> three-year Pallet transition -> Crystal Johto -> new Indigo League -> S.S. Aqua -> Kanto Revisited -> Mt. Silver -> #001-251 endgame.'
)
anchor = '## Prototype v0.1 scope\n'
if text.count(anchor) != 1:
    raise SystemExit('master rules: prototype scope anchor drifted')
architecture = '''## Campaign and generation boundary\n- `VAR_KANTO_JOHTO_CAMPAIGN` is the persistent campaign-phase foundation; zero is intentionally Yellow Kanto for new saves.\n- Yellow Kanto and pre-Johto Sevii expose only Pokémon #001-151. Sevii may foreshadow Johto through Celio, Bill, Oak, Rocket research, and regional communications but must not reveal Gen II species in wild encounters or Trainer teams.\n- Gen II Pokémon and Gen II cross-generation evolutions unlock when the Johto campaign begins. Johto retains its canonical mixed Gen I/Gen II ecosystem.\n- After Johto completion and the return to Kanto, both generations may coexist in Kanto/Sevii through rematches, migration, swarms, breeding, and postgame events.\n- The protagonist reaches Johto with only Partner Pikachu; the Kanto collection remains preserved for later interregional reconnection through Bill's Ecruteak/Time Capsule storyline.\n\n## Legendary and mythical continuity\n- Articuno remains at Seafoam Islands, Zapdos at the Power Plant, and Moltres at Victory Road, following Yellow's Kanto geography.\n- Mewtwo remains a Cerulean Cave endgame encounter after the appropriate Kanto/Sevii progression.\n- Mew receives an in-game Old Sea Map/Faraway Island-style quest; no external event is required.\n- Johto legendary progression follows Crystal for Suicune/Eusine, Gold/Silver/Crystal locations for Ho-Oh and Lugia, and an in-game GS Ball/Ilex Forest path for Celebi.\n\n'''
text = text.replace(anchor, architecture + anchor, 1)
text = text.replace(
    '- Starter Pikachu is level 5 with Thunder Shock and Growl only, uses the Yellow stat/growth/level-up progression contract above, and remains a normal trainable/boxable/evolvable owned Pokémon.',
    '- Oak gives the level-5 story Partner as `SPECIES_PIKACHU_STARTER`; it remains Pokédex #025 but has Partner-only modern stats and separation/evolution/trade restrictions. Ordinary Pikachu are unaffected.'
)
text = text.replace(
    '- After the rival leaves, Pikachu comes back out of its Poké Ball; Oak explains its behavior and the existing lead-party follower system activates at that story beat.',
    '- After the rival leaves, Pikachu comes back out of its Poké Ball; Oak explains its behavior and the persistent Partner follower flag activates at that exact story beat.'
)
write(path, text)

# 8. Guard the architecture we expect to exist after the patch.
checks = {
    'src/data/pokemon/species_info/gen_1_families.h': ['.baseDefense   = PIKACHU_DEFENSE', '[SPECIES_PIKACHU_STARTER]', '.cannotBeTraded = TRUE'],
    'data/maps/PalletTown_ProfessorOaksLab_Frlg/scripts.inc': ['SPECIES_PIKACHU_STARTER', 'FLAG_PARTNER_PIKACHU_FOLLOWING'],
    'src/event_object_movement.c': ['GetPartnerAwareFollowingMon', 'FLAG_PARTNER_PIKACHU_FOLLOWING'],
    'src/pokemon_storage_system.c': ['MSG_PARTNER_PIKACHU_STAYS', 'MSTATE_ERROR_PARTNER_PIKACHU'],
    'include/constants/vars_frlg.h': ['VAR_KANTO_JOHTO_CAMPAIGN'],
    'include/constants/kanto_johto.h': ['PARTNER_PIKACHU_JOHTO_MIN_CHALLENGE_LEVEL 30'],
}
for file, needles in checks.items():
    content = read(file)
    for needle in needles:
        if needle not in content:
            raise SystemExit(f'{file}: validation missing {needle!r}')

print('Stage 1A guarded patch applied successfully.')
