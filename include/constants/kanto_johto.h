#ifndef GUARD_CONSTANTS_KANTO_JOHTO_H
#define GUARD_CONSTANTS_KANTO_JOHTO_H

// Persistent campaign phases stored in VAR_KANTO_JOHTO_CAMPAIGN.
// The default zero value intentionally means the initial Yellow Kanto campaign.
// Future region-gated systems should key off this campaign phase instead of
// scattering independent Kanto/Johto progression assumptions across the engine.
enum KantoJohtoCampaignPhase
{
    KANTO_JOHTO_CAMPAIGN_YELLOW_KANTO = 0,
    KANTO_JOHTO_CAMPAIGN_SEVII,
    KANTO_JOHTO_CAMPAIGN_TIME_SKIP,
    KANTO_JOHTO_CAMPAIGN_JOHTO,
    KANTO_JOHTO_CAMPAIGN_KANTO_RETURN,
    KANTO_JOHTO_CAMPAIGN_ENDGAME,
};

#define KANTO_JOHTO_GEN2_UNLOCK_PHASE KANTO_JOHTO_CAMPAIGN_JOHTO
#define PARTNER_PIKACHU_JOHTO_MIN_CHALLENGE_LEVEL 30

#endif // GUARD_CONSTANTS_KANTO_JOHTO_H
