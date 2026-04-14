# Input Strategy Comparison (for report)

## Experiment 1: Direct single input read
- Setup: process only one key per frame.
- Observation: under fast key presses, direction updates are often missed or feel unresponsive.
- Risk: when timing aligns badly, users report accidental self-collision feeling.

## Experiment 2: Input queue playback
- Setup: enqueue all direction keys and execute in order.
- Observation: historical inputs continue to execute even when user intention has changed.
- Drawback: visible control latency, especially after rapid multi-key input.

## Experiment 3: Latest legal input override (final)
- Setup: read all keys in the current frame, keep only the last legal direction.
- Legality rule: if input is opposite to current direction, ignore it.
- Result: fastest response, no stale input playback, and robust against 180-degree reversal bug.

## Why not use a queue?
- Queue preserves history, but snake control is intention-driven, not history-driven.
- In fast gameplay, executing old inputs makes controls feel delayed.

## Why not keep only the first input in a frame?
- First-input strategy can ignore the most recent user intent within the same frame.
- Latest legal override better matches player expectation and improves responsiveness.
