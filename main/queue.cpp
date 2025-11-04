class Elevator {
private:
    float currentPosition;
    float targetPosition;
    int direction;  // 1 = Up, 0 = Stationary, -1 = Down

public:
    Elevator() {
        currentPosition = 0;
        targetPosition = 0;
        direction = 0;
    }

    void processRequest(float requestPosition, int requestDirection) {
        // If elevator is stationary, accept any request
        if (direction == 0) {
            targetPosition = requestPosition;
            direction = (requestPosition > currentPosition) ? 1 : -1;
        }
        // If moving up, only accept requests above current position or in same direction
        else if (direction == 1) {
            if (requestPosition > currentPosition && requestPosition < targetPosition) {
                targetPosition = requestPosition;
            }
        }
        // If moving down, only accept requests below current position or in same direction
        else if (direction == -1) {
            if (requestPosition < currentPosition && requestPosition > targetPosition) {
                targetPosition = requestPosition;
            }
        }
    }

    void updatePosition(float newPosition) {
        currentPosition = newPosition;
        
        // Check if we reached target
        if (abs(currentPosition - targetPosition) < 0.1) {
            direction = 0;  // Stop at target
        }
    }

    // Getters
    float getCurrentPosition() { return currentPosition; }
    float getTargetPosition() { return targetPosition; }
    int getDirection() { return direction; }
};

// Global elevator instance
Elevator elevator;

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Example usage
    if (Serial.available()) {
        float reqPosition = Serial.parseFloat();
        int reqDirection = Serial.parseInt();
        
        elevator.processRequest(reqPosition, reqDirection);
        
        Serial.print("Target: ");
        Serial.print(elevator.getTargetPosition());
        Serial.print(" Position: ");
        Serial.print(elevator.getCurrentPosition());
        Serial.print(" Direction: ");
        Serial.println(elevator.getDirection());
    }
}