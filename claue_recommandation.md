gridworld 밖에서 tabular 방법(SARSA/Q-learning/MC 등)으로 학습 가능한 고전 환경들을 카테고리별로 정리해드립니다. 상태/액션 공간이 충분히 작아서 table로 다룰 수 있는 것들 위주입니다.

### A. 카드/도박 게임 (state space가 자연스럽게 작음)

1. **Blackjack (Sutton & Barto Example 5.1, 5.3)** — state는 (현재 합, 딜러 카드, ace 보유 여부)로 약 200개 미만. MC prediction/control의 정전 예제이고, **off-policy MC를 디버깅하신 직후라 바로 재활용하기 좋습니다**.
2. **Baccarat / 간단화된 포커 변형** — 액션 수가 적은 변형(예: hit/stand만 있는 단순화 포커)이면 state 설계만 잘 하면 충분히 tabular로 가능.

### B. 고전 제어 문제 (discretize 필요)

3. **Mountain Car** — 연속 state(위치, 속도)를 적당히 binning하면 tabular Q-learning으로 풀리는 유명한 sparse-reward 문제. "산을 못 올라가서 뒤로 가속해야 한다"는 비직관적 전략이 학습되는 과정이 재밌습니다.
4. **CartPole (discretized)** — 4차원 연속 state를 좁은 bin으로 나누면 tabular로도 풀립니다. state 차원이 늘어나면 table 크기가 기하급수적으로 커지는 걸 직접 체감할 수 있어서, Deep RL 동기부여용으로도 좋습니다.
5. **Pendulum (discretized, 단순화)** — 위 두 개보다 더 어려운 연속 제어. discretization 해상도에 따라 학습 성패가 갈리는 걸 보여주는 좋은 사례.
6. **Acrobot** — 2-link 로봇 팔. state가 더 복잡해서 (각도, 각속도 2쌍) discretize 시 table이 꽤 커지지만 여전히 가능.

### C. 보드/퍼즐 게임 (작은 변형)

7. **Tic-Tac-Toe** — self-play로 학습시키는 RL 입문 단골 예제 (Sutton 1장에서 직접 언급). state 수가 충분히 작아 완전 tabular.
8. **작은 Nim 게임** — 게임 이론 + RL 교차 예제로 자주 쓰임. 최적 전략이 수학적으로 알려져 있어서 학습 결과를 검증하기 매우 쉽습니다.
9. **작은 보드의 Connect Four (예: 4x4 또는 5x4)** — 표준 7x6은 state 수가 너무 크지만, 보드를 줄이면 tabular self-play가 가능.
10. **Tower of Hanoi (3~4 disk)** — state가 disk 배치 조합으로 명확히 정의되고, 최적 해가 알려져 있어서 학습 검증이 쉬움.

### D. Bandit 계열 (가장 단순한 baseline)

11. **k-armed bandit** (지난번 말씀드린) — exploration-exploitation trade-off의 가장 순수한 형태.
12. **Contextual bandit** — context(작은 discrete state)에 따라 최적 arm이 달라지는 구조. gridworld 없이 "state 인식 + 의사결정"을 동시에 보는 중간 난이도 단계.

### E. 고전 RL 벤치마크 (작은 변형)

13. **Jack's Car Rental (Sutton & Barto Example 4.2)** — DP(Policy Iteration) 예제로 더 유명하지만, MC/TD로도 풀 수 있습니다. state는 (location A 차량 수, location B 차량 수)로 작음. **Poisson 분포 기반 stochastic demand**라서 stochastic 환경 다루는 연습에 좋습니다.
14. **Gambler's Problem (Example 4.3)** — 동전 던지기로 돈을 베팅하는 문제. state는 현재 자본(0~100)뿐이라 매우 작고, 최적 정책이 비직관적(fractal-like)인 게 흥미롭습니다.
15. **Random Walk (Example 6.2)** — 가장 단순한 TD vs MC 비교용 예제. 1차원 상태 5~7개. TD(λ)나 n-step method 디버깅에 최적.

### F. 카드/주사위 기반 미니게임

16. **Pig (주사위 게임)** — state는 (현재 점수, 턴 점수)로 작고, 최적 정책이 알려진 threshold 전략과 거의 일치해서 검증이 쉽습니다.
17. **Easy21 (David Silver RL 강의 과제)** — Blackjack의 단순화 변형. 이미 유명한 강의 과제라 참고 자료가 많아서 본인 구현 검증하기 좋습니다.

---

**추천 우선순위**:
- **Blackjack**: off-policy MC를 막 디버깅하셨으니 바로 이어서 검증하기 가장 자연스럽습니다.
- **Random Walk**: n-step method가 로드맵에 있으니, n 값에 따른 bias-variance trade-off를 가장 간단하게 보여주는 예제입니다.
- **Mountain Car**: tabular의 한계(discretization 해상도 문제)를 체감하면서 Deep RL로 넘어가는 다리 역할로 적합합니다.
- **Jack's Car Rental / Gambler's Problem**: DP를 안 다루셨더라도, stochastic transition을 다루는 MC/TD 연습으로 유효합니다.