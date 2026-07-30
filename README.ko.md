# Unreal Engine용 4K Screenshot Toolbar

[English README](README.md)

**4K Screenshot Toolbar**는 Unreal Editor의 활성 레벨 뷰포트를 정확한
**3840 × 2160 PNG**로 저장해 주는 가벼운 에디터 플러그인입니다. 툴바의
**4K Shot** 버튼을 누르거나 **F12**를 누르면 바로 촬영됩니다.

## 주요 기능

- 활성 Level Editor 뷰포트를 한 번에 4K로 캡처
- 기본 단축키 F12 제공 및 Unreal Editor에서 자유롭게 변경 가능
- 촬영 순간 Game View를 자동으로 적용해 에디터 아이콘과 기즈모 숨김
- 촬영이 끝나면 기존 뷰포트 상태 자동 복원
- Unreal Engine의 기본 스크린샷 완료 알림 사용
- 별도의 성공음이나 중복 성공 알림 없이 조용하게 동작
- Editor 전용 모듈로, 패키징된 게임에는 포함되지 않음

Windows Editor에서 스크린샷은 기본적으로 다음 폴더에 저장됩니다.

```text
<프로젝트>/Saved/Screenshots/WindowsEditor/
```

## 호환성

- Unreal Engine 5.8 / Windows에서 테스트 완료
- 다른 Unreal Engine 5.x 버전은 아직 검증하지 않았습니다

## 설치 방법

1. [최신 릴리스](https://github.com/fairypark/unreal-4k-screenshot-toolbar/releases/latest)에서
   `HighResScreenshotToolbar-v1.0.0-UE5.8-Win64.zip`을 내려받습니다.
2. 압축 안의 `HighResScreenshotToolbar` 폴더를 프로젝트의 `Plugins` 폴더에 넣습니다.

   ```text
   <프로젝트>/Plugins/HighResScreenshotToolbar/
   ```

3. 프로젝트를 엽니다.
4. 필요하면 `편집 > 플러그인`에서 **4K Screenshot Toolbar**를 활성화합니다.
5. 안내에 따라 에디터를 재시작합니다.

바로 설치 가능한 패키지는 Unreal Engine 5.8 / Win64용이며 소스도 함께
포함합니다. 다른 엔진 버전에서는 저장소를 복제하거나 GitHub 소스 압축을
내려받아 해당 버전에 맞는 C++ 환경으로 빌드해야 합니다.

## 사용 방법

Level Editor 뷰포트를 활성화한 뒤 다음 중 하나를 실행합니다.

- 툴바의 **4K Shot** 버튼 클릭
- **F12** 누르기

단축키를 바꾸려면 `편집 > 키보드 단축키` (`Ctrl+Alt+K`)를 열고
`4K Shot`을 검색해 원하는 키를 지정하면 됩니다.

## 라이선스

[MIT License](LICENSE)로 공개합니다.

Unreal Engine은 Epic Games, Inc.의 상표 또는 등록 상표입니다. 이 프로젝트는
Epic Games와 제휴하거나 Epic Games의 보증을 받은 프로젝트가 아닙니다.
