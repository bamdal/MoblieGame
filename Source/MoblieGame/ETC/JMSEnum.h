#pragma once

UENUM(BlueprintType)
enum class DummyState : uint8
{
	Chaser =0,
	Runner_Duck =1,
	Runner_Deer =2,
	Runner_Statue =3,
	Runner_None =4,
	
};

UENUM(BlueprintType)
enum class EButtonState : uint8
{
	Normal,      // 기본 상태
	Selected,    // 플레이어가 선택한 상태
	Disabled     // 술래 버튼으로 선택된 상태 (모든 플레이어에게 적용)
};