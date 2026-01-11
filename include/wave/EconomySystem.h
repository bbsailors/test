#pragma once

#include <functional>

// ============================================================
// EconomySystem: Manages game resources (Gold/Energy)
// ============================================================
// This system handles all monetary transactions in the game:
// - Starting resources
// - Rewards from killing enemies
// - Costs for building/upgrading towers
// ============================================================

class EconomySystem {
public:
    EconomySystem();
    
    // -------------------- Initialization --------------------
    
    // Initialize economy with starting gold
    void Init(int startingGold);
    
    // Reset to initial state
    void Reset();
    
    // -------------------- Transactions --------------------
    
    // Add gold (from enemy kills, wave completion bonuses, etc.)
    void AddGold(int amount);
    
    // Attempt to spend gold. Returns true if successful, false if insufficient funds.
    bool SpendGold(int amount);
    
    // Check if player can afford a purchase
    bool CanAfford(int cost) const;
    
    // -------------------- Getters --------------------
    
    int GetGold() const { return gold; }
    int GetTotalEarned() const { return totalEarned; }
    int GetTotalSpent() const { return totalSpent; }
    
    // -------------------- Callbacks --------------------
    
    // Called when gold amount changes (for UI updates)
    void SetOnGoldChanged(std::function<void(int)> callback);
    
    // Called when a purchase fails due to insufficient funds
    void SetOnInsufficientFunds(std::function<void(int cost, int available)> callback);
    
    // -------------------- Wave Bonuses --------------------
    
    // Award bonus gold for completing a wave
    void AwardWaveCompletionBonus(int waveNumber);
    
    // Award bonus gold for completing all waves (victory)
    void AwardVictoryBonus();
    
private:
    int gold;              // Current gold amount
    int totalEarned;       // Total gold earned (statistics)
    int totalSpent;        // Total gold spent (statistics)
    int startingAmount;    // Initial gold value (for reset)
    
    // Callbacks
    std::function<void(int)> onGoldChanged;
    std::function<void(int, int)> onInsufficientFunds;
    
    // Constants
    static constexpr int WAVE_COMPLETION_BASE_BONUS = 25;    // Base bonus per wave
    static constexpr int WAVE_COMPLETION_MULTIPLIER = 5;     // Extra per wave number
    static constexpr int VICTORY_BONUS = 500;                // Bonus for winning
};
