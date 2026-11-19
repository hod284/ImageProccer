## 개발 환경
- Visual Studio 2022 호환 MSVC v143, Windows SDK
- C++17, x64 / Release (디버깅 시 Debug)
- 외부 라이브러리 의존성 없음

## 구현 항목
- Grayscale 변환
- 밝기 조절
- 대비 조절
- 이진화
- 흐림 처리(3×3 합성곱 이용)
- 선명도 개선(3×3 합성곱 이용)
- 히스토그램
- 크롭
- 리사이즈
- 상하 반전
- 좌우 반전
## 추가 기능
- 필터별 처리 시간, 파라미터 및 성공·실패 로그 출력
- 사용자 지정 순서의 필터 파이프라인 체인

## 실행 명령어 

 *가장 먼저 powershell을 켜서 cd를 이용하여 ImageProcessor.exe 가 있는 폴더 전에 전 폴더의 파일명을 붙여서 파일 주소를 이동 한 뒤 밑에 있는 명령어를 파워셀에 붙여 넣으세요 
  
 # Grayscale 변환
  ```powershell
   .\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Resource\1_astronaut_grayscale.bmp --filter grayscale
   .\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Resource\1_astronaut_grayscale.bmp -f grayscale
  ```

 # 밝기 조절
  ```powershell
   .\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Resource\1_astronaut_brightness.bmp --filter brightness:50
   .\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Resource\1_astronaut_brightness.bmp -f brightness:50
  ```

  # 대비 조절
  ```powershell
   .\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Resource\1_astronaut_contrast.bmp --filter contrast:1.5
   .\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Resource\1_astronaut_contrast.bmp -f contrast:1.5
  ```

  # 이진화
  ```powershell
   .\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Resource\1_astronaut_threshold.bmp --filter threshold:128
   .\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Resource\1_astronaut_threshold.bmp -f threshold:128
  ```

  # 흐림 처리(3×3 합성곱 이용)
  ```powershell
   .\x64\Release\ImageProcessor.exe --input .\Resource\2_coffee.bmp --output .\Resource\2_coffee_blur.bmp --filter blur
   .\x64\Release\ImageProcessor.exe --input .\Resource\2_coffee.bmp --output .\Resource\2_coffee_blur.bmp -f blur
  ```

   # 선명도 개선(3×3 합성곱 이용)
  ```powershell
   .\x64\Release\ImageProcessor.exe --input .\Resource\2_coffee.bmp --output .\Resource\2_coffee_sharpen.bmp --filter sharpen
   .\x64\Release\ImageProcessor.exe --input .\Resource\2_coffee.bmp --output .\Resource\2_coffee_sharpen.bmp -f sharpen
  ```

  # 히스토그램
  ```powershell
   .\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Resource\1_astronaut_histogram.bmp --filter histogram
   .\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Resource\1_astronaut_histogram.bmp -f histogram
  ```

  # 크롭
  ```powershell
  .\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Resource\3_chelsea_cat_crop.bmp --filter crop:10:20:200:200
  .\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Resource\3_chelsea_cat_crop.bmp -f crop:10:20:200:200
  ```

   # 리사이즈
  ```powershell
  .\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Resource\3_chelsea_cat_resize.bmp --filter resize:800:600
  .\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Resource\3_chelsea_cat_resize.bmp -f resize:800:600
  ```

   # 상하 반전
  ```powershell
  .\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Resource\3_chelsea_cat_vertical.bmp --filter flipVertical
  .\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Resource\3_chelsea_cat_vertical.bmp -f flipVertical
  ```

  # 좌우 반전
  ```powershell
  .\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Resource\3_chelsea_cat_horizontal.bmp --filter flipHorizontal
  .\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Resource\3_chelsea_cat_horizontal.bmp -f flipHorizontal
  ```

   # 필터 파이프라인 체인 (고급)
  ```powershell
   .\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Resource\3_chelsea_cat_pipeline.bmp --pipeline "grayscale,blur,threshold:128,histogram"
  ```

   # 멀티스레드 병렬 처리 
  ```powershell
   .\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Resource\1_astronaut_parallel.bmp --pipeline "resize:1920:1080,blur,sharpen"
  ```


