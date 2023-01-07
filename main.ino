#define SIGNAL_OFF 0
#define SIGNAL_TEST 1
#define SIGNAL_HP0 2
#define SIGNAL_HP1 3
#define SIGNAL_HP2 4
#define NUM_SIGNAL_PATTERNS 5

#define NUMBER_OF_SIGNALS 1

#define REFRESH_RATE_MS 10
#define LONG_PRESS_TIME_MS 250
#define BUTTON_OFF_TOLERANCE_MS 10

#define BLINK_OFF_TIME_MS 1000
#define BLINK_ON_TIME_MS 1000

const int signalButtons[NUMBER_OF_SIGNALS][2] = {{DD2, DD3}};
const int signalOutputs[NUMBER_OF_SIGNALS][3] = {{A0, A1, A2}};
const int signalPatterns[NUM_SIGNAL_PATTERNS][3] = {
    {LOW, LOW, LOW}, 
    {HIGH, HIGH, HIGH},
    {LOW, HIGH, LOW},
    {HIGH, LOW, LOW},
    {HIGH, LOW, HIGH},
};
const int signalLongPressConfig[NUMBER_OF_SIGNALS][2] = {
    {SIGNAL_HP2, SIGNAL_OFF}
};
const int signalShortPressConfig[NUMBER_OF_SIGNALS][2] = {
    {SIGNAL_HP1, SIGNAL_HP0}
};

const int buttonOffToleranceCount = BUTTON_OFF_TOLERANCE_MS / REFRESH_RATE_MS;
const int buttonLongPressCount = LONG_PRESS_TIME_MS / REFRESH_RATE_MS;
const int blinkOffCount = BLINK_OFF_TIME_MS / REFRESH_RATE_MS;
const int blinkOnCount = BLINK_ON_TIME_MS / REFRESH_RATE_MS;

const int confirmLED = LED_BUILTIN;

int signalButtonsPressed[NUMBER_OF_SIGNALS][2] = {{LOW, LOW}};
int signalButtonsHoldCount[NUMBER_OF_SIGNALS][2] = {{0, 0}};
int signalButtonsReleaseCount[NUMBER_OF_SIGNALS][2] = {{0, 0}};
int signalStates[NUMBER_OF_SIGNALS] = {SIGNAL_TEST};

int blinkCount[NUMBER_OF_SIGNALS] = {0};

void setup()
{
    for(int i = 0; i < NUMBER_OF_SIGNALS; i++)
    {
        pinMode(signalButtons[i][0], INPUT);
        pinMode(signalButtons[i][1], INPUT);

        pinMode(signalOutputs[i][0], OUTPUT);
        pinMode(signalOutputs[i][1], OUTPUT);
        pinMode(signalOutputs[i][2], OUTPUT);
    }
}

void loop()
{
    digitalWrite(confirmLED, LOW);
    readButtonStates();
    handleButtonStates();
    outputStates();
    delay(REFRESH_RATE_MS);
}

void outputStates()
{
    for(int i = 0; i < NUMBER_OF_SIGNALS; i++)
    {
        int state = signalStates[i];
        const int *pattern = signalPatterns[state];

        digitalWrite(signalOutputs[i][0], pattern[0]);
        digitalWrite(signalOutputs[i][1], pattern[1]);
        digitalWrite(signalOutputs[i][2], pattern[2]);
    }
}

void buttonShortPress(int signal, int button)
{
    digitalWrite(confirmLED, HIGH);
    signalStates[signal] = signalShortPressConfig[signal][button];
}

void buttonLongPress(int signal, int button)
{
    digitalWrite(confirmLED, HIGH);
    signalStates[signal] = signalLongPressConfig[signal][button];
}

void handleButtonStates()
{
    for(int i = 0; i < NUMBER_OF_SIGNALS; i++)
    {
        handleSignalButtonState(i, 0);
        handleSignalButtonState(i, 1);
    }
}

void handleSignalButtonState(int signal, int button)
{
    if(signalButtonsPressed[signal][button] == HIGH)
    {
        signalButtonsHoldCount[signal][button]++;
        signalButtonsReleaseCount[signal][button] = 0;
    }
    else if(signalButtonsHoldCount[signal][button] > 0 && signalButtonsReleaseCount[signal][button] < buttonOffToleranceCount)
    {
        signalButtonsReleaseCount[signal][button]++;
    }
    else if(signalButtonsHoldCount[signal][button] > 0)
    {
        if(signalButtonsHoldCount[signal][button] >= buttonLongPressCount)
            buttonLongPress(signal, button);
        else
            buttonShortPress(signal, button);

        signalButtonsHoldCount[signal][button] = 0;
        signalButtonsReleaseCount[signal][button] = 0;
    }
}

void readButtonStates()
{
    for(int i = 0; i < NUMBER_OF_SIGNALS; i++)
    {
        signalButtonsPressed[i][0] = digitalRead(signalButtons[i][0]);
        signalButtonsPressed[i][1] = digitalRead(signalButtons[i][1]);
    }
}
