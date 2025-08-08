# 커스텀 한국 장기 게임

## 1. 프로젝트 소개

이 프로젝트는 언리얼 엔진을 사용하여 개발한 3D 턴제 멀티플레이어 한국 장기 게임입니다. 온라인을 통해 두 명의 플레이어가 서로 대전할 수 있으며, 각기 다른 능력을 가진 다양한 기물, 점수 및 부활 시스템을 통해 전략적인 재미를 느낄 수 있도록 구현했습니다.

---

## 2. 주요 기능

*   **멀티플레이어 지원:** 클라이언트-서버 아키텍처를 기반으로 2인 온라인 대전을 지원합니다.
*   **턴제 시스템:** 플레이어들은 차례대로 자신의 기물을 움직이고, 공격하며, 스킬을 사용합니다.
*   **개성 있는 기물:** 차(車), 포(包), 마(馬), 상(象), 사(士), 졸(卒), 장(將) 등 각기 다른 이동 및 공격, 스킬 능력을 가진 다양한 장기 기물들을 구현했습니다.
*   **스킬 시스템:** 특정 기물들은 턴 동안 사용할 수 있는 고유한 액티브 스킬을 가지고 있습니다.
*   **점수 및 부활 시스템:** 상대방의 기물을 잡으면 점수를 획득하며, 이 점수를 사용하여 잡힌 자신의 기물을 부활시킬 수 있어 전략적인 깊이를 더합니다.
*   **게임 흐름:** 메뉴, 로비, 인게임, 게임 오버로 이어지는 자연스러운 게임 플로우를 구현했습니다.
*   **UI / 시각 효과:** UMG를 활용한 UI와 파티클 시스템을 이용한 각종 효과로 시각적 경험을 향상시킵니다.

---

## 3. 아키텍처 및 클래스 상세 설명

이 프로젝트는 언리얼 엔진의 게임플레이 프레임워크를 기반으로 설계되었으며, 각 클래스는 명확한 역할을 가지고 유기적으로 상호작용합니다.

### 3.1. 클래스 상속 구조

주요 클래스들의 상속 관계는 다음과 같습니다.

*   `ACharacter`
    *   **`AKCPiece`** (모든 기물의 부모 클래스)
        *   `AChaPiece`, `AJangKCPiece`, `AJolPiece`, `AMaPiece`, `APoPiece`, `ASangPiece`, `ASaPiece`
*   `AGameModeBase`
    *   `AKoreanChessGameMode` (인게임 로직 관리)
    *   `ALobbyGameMode` (로비 로직 관리)
    *   `AMenuGameMode` (메뉴 화면 관리)
*   `AGameStateBase`
    *   `AKCGameStateBase` (게임 전체 상태 관리 및 복제)
*   `APlayerController`
    *   `AKoreanChessPlayerController` (인게임 플레이어 입력 및 서버 통신)
    *   `ALobbyPlayerController` (로비 플레이어 입력 및 서버 통신)
*   `AAIController`
    *   `APieceController` (기물의 실제 이동 및 행동 제어)
*   `AActor`
    *   `AKCPointActor` (장기판의 각 칸)
    *   `AKCDeathBoardActor` (죽은 기물이 놓이는 칸)
    *   `AChaProjectile` (투사체)
    *   `AKCSpawnManager` (기물 및 보드 생성 관리)
*   `UUserWidget`
    *   `UMainGameUserWidget`, `ULobbyUserWidget`, `UGameOverUserWidget` 등 다수의 UI 클래스

### 3.2. 주요 클래스 기능 설명

#### **`AKCPiece` (기물 베이스 클래스)**
*   모든 장기 기물의 부모 클래스로, 기물이 가져야 할 공통 속성과 기능을 정의합니다.
*   **주요 속성:** `MaxHP`, `CurrentHP`, `Damage` (공격력), `Team` (소속 팀), `State` (현재 상태: IDLE, ATTACK 등), `Kind` (기물 종류).
*   **주요 기능:**
    *   `TakeDamage`: 피해를 받았을 때 HP를 감소시키고, HP가 0 이하가 되면 죽음 로직을 처리합니다.
    *   `AttackStart`, `ActiveStart`: 자식 클래스에서 구현될 공격 및 스킬 사용의 시작을 알리는 순수 가상 함수입니다.
    *   `ClickedCapsule`, `BeginOverCapsule`: 플레이어의 마우스 클릭 및 오버 이벤트에 반응하여 기물 선택, 정보 표시 등의 상호작용을 처리합니다.
    *   상태에 따라 외곽선 색상을 변경(`ChangeColor`)하거나 파티클을 표시(`UpdateParticle`)하여 시각적인 피드백을 제공합니다.

#### **`AKoreanChessGameMode` (메인 게임 모드)**
*   실제 장기 게임의 규칙과 흐름을 총괄합니다.
*   **주요 기능:**
    *   `PostLogin`: 플레이어가 서버에 접속했을 때 컨트롤러를 배열에 저장하고 초기 위치를 설정합니다.
    *   `ReceiveMessage`: `PlayerController`로부터 받은 메시지(이동, 공격, 턴 종료 등)를 분석하여 해당 로직을 실행합니다.
    *   `MoveSelectedActor`, `AttackSelectedActor`: `PieceController`를 통해 실제 기물의 이동과 공격을 지시합니다.
    *   `ProgressNextTurn`: 턴을 다음 플레이어에게 넘기고, 제한 시간을 초기화합니다.
    *   `CheckCurrentTurn`: 기물의 행동(이동, 공격, 죽음)이 완료되었는지 체크하고, 모든 행동이 끝나면 플레이어에게 다시 입력을 허용합니다.
    *   `PlaySequence`: 장(Jang) 기물이 잡혔을 때, 게임 오버 시네마틱 시퀀스를 재생합니다.

#### **`AKoreanChessPlayerController` (인게임 플레이어 컨트롤러)**
*   플레이어의 입력을 받아 서버로 전달하고, 서버의 응답을 받아 게임에 반영하는 역할을 합니다.
*   **주요 기능:**
    *   `TrySendMessage`: 플레이어의 입력(기물 선택, 이동 위치 클릭 등)을 검증하고, 유효할 경우 `SendMessage` RPC를 호출하여 서버에 알립니다.
    *   `SendMessage` (Server RPC): 서버의 `GameMode`로 플레이어의 행동 요청을 전달합니다.
    *   `RecieveMessage` (Client RPC): 서버로부터 받은 게임 상태 변경(게임 시작, 턴 변경, 게임 오버 등)을 클라이언트에 적용하고, HUD를 업데이트합니다.
    *   `SelectPiece`: 플레이어가 선택한 기물을 관리하고, 이전에 선택된 기물은 선택 해제합니다.

#### **`AKCGameStateBase` (게임 상태)**
*   모든 플레이어가 공유해야 할 게임의 중요 상태 정보를 담고 있으며, 이 정보들은 서버에서 클라이언트로 자동 복제(Replicate)됩니다.
*   **주요 복제 속성:** `CurrentTurn` (현재 턴인 팀), `PointMap` (장기판 각 칸의 정보), `FirstOrderScore`/`SecondOrderScore` (각 팀의 점수), `LimitTime` (남은 턴 시간), `bIsAlreadyMove`/`bIsAlreadySkill` (이번 턴에 이동/스킬 사용 여부).
*   **주요 기능:**
    *   `PieceSelect`: 선택된 기물이 이동하거나 공격할 수 있는 위치를 `MACalculate` 유틸리티를 통해 계산하고, 해당 위치들을 시각적으로 표시합니다.
    *   `ChangeTurn`: 턴을 넘기고 관련 상태(이동/스킬 사용 여부)를 초기화합니다.
    *   `IncreaseScore`: 기물을 잡았을 때 점수를 증가시킵니다.

#### **`MACalculate` (이동/공격 가능 범위 계산 유틸리티)**
*   특정 기물이 현재 위치에서 이동하거나 공격할 수 있는 모든 경우의 수를 계산하는 순수 C++ 클래스입니다.
*   각 기물의 고유한 행마법(예: 마(Ma)의 겹마, 상(Sang)의 길, 포(Po)의 넘기 등)을 정교하게 계산하여 `FMAResultData` 구조체에 담아 반환합니다.
*   궁성(Castle) 내에서의 특수 이동 규칙 또한 별도로 처리합니다.

---
