#include "global.h"
#include "event_data.h"
#include "follower_npc.h"
#include "pokemon.h"
#include "script.h"
#include "constants/flags.h"
#include "constants/species.h"

// Yellow-only Partner checks must never collapse the canonical starter identity
// into an ordinary Pikachu species check.  Require the story-following state,
// the live follower object, and the dedicated Partner species in the party.
void GoldenYellow_CheckPartnerPikachuFollowing(void)
{
    u8 i;

    gSpecialVar_Result = FALSE;

    if (!FlagGet(FLAG_PARTNER_PIKACHU_FOLLOWING) || !PlayerHasFollowerNPC())
        return;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES) == SPECIES_PIKACHU_STARTER)
        {
            gSpecialVar_Result = TRUE;
            return;
        }
    }
}
