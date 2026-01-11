#pragma once

// ============================================================
// Timer: Simple timer utility for game logic
// ============================================================

class Timer {
public:
    Timer()
        : duration(0.0f)
        , elapsed(0.0f)
        , running(false)
        , looping(false)
    {}
    
    Timer(float durationSeconds, bool loop = false)
        : duration(durationSeconds)
        , elapsed(0.0f)
        , running(false)
        , looping(loop)
    {}
    
    // Start the timer
    void Start() {
        elapsed = 0.0f;
        running = true;
    }
    
    // Start with a new duration
    void Start(float durationSeconds) {
        duration = durationSeconds;
        Start();
    }
    
    // Stop the timer
    void Stop() {
        running = false;
    }
    
    // Reset without starting
    void Reset() {
        elapsed = 0.0f;
    }
    
    // Pause/Resume
    void Pause() { running = false; }
    void Resume() { running = true; }
    
    // Update the timer, returns true if just finished
    bool Update(float dt) {
        if (!running) return false;
        
        elapsed += dt;
        
        if (elapsed >= duration) {
            if (looping) {
                elapsed -= duration;  // Keep overflow for precision
                return true;  // Triggered
            } else {
                elapsed = duration;
                running = false;
                return true;  // Finished
            }
        }
        
        return false;
    }
    
    // Check if timer is done (non-looping only)
    bool IsFinished() const {
        return !running && elapsed >= duration;
    }
    
    // Check if running
    bool IsRunning() const { return running; }
    
    // Get progress (0.0 to 1.0)
    float GetProgress() const {
        if (duration <= 0.0f) return 1.0f;
        return elapsed / duration;
    }
    
    // Get remaining time
    float GetRemaining() const {
        return duration - elapsed;
    }
    
    // Get elapsed time
    float GetElapsed() const { return elapsed; }
    
    // Get duration
    float GetDuration() const { return duration; }
    
    // Set looping
    void SetLooping(bool loop) { looping = loop; }
    bool IsLooping() const { return looping; }
    
private:
    float duration;
    float elapsed;
    bool running;
    bool looping;
};

// ============================================================
// Cooldown: Specialized timer for ability cooldowns
// ============================================================
class Cooldown {
public:
    Cooldown() : cooldownTime(1.0f), currentTime(0.0f) {}
    Cooldown(float time) : cooldownTime(time), currentTime(0.0f) {}
    
    // Update cooldown
    void Update(float dt) {
        if (currentTime > 0.0f) {
            currentTime -= dt;
            if (currentTime < 0.0f) {
                currentTime = 0.0f;
            }
        }
    }
    
    // Trigger the cooldown
    void Trigger() {
        currentTime = cooldownTime;
    }
    
    // Check if ready (cooldown finished)
    bool IsReady() const {
        return currentTime <= 0.0f;
    }
    
    // Get remaining cooldown
    float GetRemaining() const { return currentTime; }
    
    // Get cooldown progress (1.0 = ready, 0.0 = just triggered)
    float GetProgress() const {
        if (cooldownTime <= 0.0f) return 1.0f;
        return 1.0f - (currentTime / cooldownTime);
    }
    
    // Set cooldown time
    void SetCooldownTime(float time) { cooldownTime = time; }
    
    // Reset cooldown (make ready immediately)
    void Reset() { currentTime = 0.0f; }
    
private:
    float cooldownTime;
    float currentTime;
};
