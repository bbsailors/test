#include "wave/EconomySystem.h"
#include <algorithm>

// ============================================================
// Constructor
// ============================================================
EconomySystem::EconomySystem()
    : gold(0)
    , totalEarned(0)
    , totalSpent(0)
    , startingAmount(0)
{
}

// ============================================================
// Initialization
// ============================================================
void EconomySystem::Init(int startingGold)
{
    startingAmount = startingGold;
    gold = startingGold;
    totalEarned = startingGold;
    totalSpent = 0;
    
    // Notify listeners of initial gold
    if (onGoldChanged) {
        onGoldChanged(gold);
    }
}

void EconomySystem::Reset()
{
    gold = startingAmount;
    totalEarned = startingAmount;
    totalSpent = 0;
    
    if (onGoldChanged) {
        onGoldChanged(gold);
    }
}

// ============================================================
// Transactions
// ============================================================
void EconomySystem::AddGold(int amount)
{
    if (amount <= 0) {
        return; // Ignore non-positive amounts
    }
    
    gold += amount;
    totalEarned += amount;
    
    if (onGoldChanged) {
        onGoldChanged(gold);
    }
}

bool EconomySystem::SpendGold(int amount)
{
    if (amount <= 0) {
        return true; // Free items always succeed
    }
    
    if (!CanAfford(amount)) {
        // Notify about insufficient funds
        if (onInsufficientFunds) {
            onInsufficientFunds(amount, gold);
        }
        return false;
    }
    
    gold -= amount;
    totalSpent += amount;
    
    if (onGoldChanged) {
        onGoldChanged(gold);
    }
    
    return true;
}

bool EconomySystem::CanAfford(int cost) const
{
    return gold >= cost;
}

// ============================================================
// Callbacks
// ============================================================
void EconomySystem::SetOnGoldChanged(std::function<void(int)> callback)
{
    onGoldChanged = std::move(callback);
}

void EconomySystem::SetOnInsufficientFunds(std::function<void(int, int)> callback)
{
    onInsufficientFunds = std::move(callback);
}

// ============================================================
// Bonuses
// ============================================================
void EconomySystem::AwardWaveCompletionBonus(int waveNumber)
{
    // Bonus increases with wave number
    // Wave 1: 25 + 5 = 30 gold
    // Wave 5: 25 + 25 = 50 gold
    // Wave 10: 25 + 50 = 75 gold
    int bonus = WAVE_COMPLETION_BASE_BONUS + (waveNumber * WAVE_COMPLETION_MULTIPLIER);
    AddGold(bonus);
}

void EconomySystem::AwardVictoryBonus()
{
    AddGold(VICTORY_BONUS);
}
