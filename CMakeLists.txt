cmake_minimum_required(VERSION 3.21)
project(j18314)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(j18314
  main.cpp
  WindowManager.cpp

  # IDE表示用に入れたければヘッダを列挙してOK
  # WindowManager.h
)

target_include_directories(j18314 PRIVATE
  /usr/include/opengl
)

if(EMSCRIPTEN)
  # 出力先ディレクトリ
  set(OUTDIR "${CMAKE_BINARY_DIR}/build")
  file(MAKE_DIRECTORY "${OUTDIR}")

  # ターゲット名は index.html にしたい
  set_target_properties(j18314 PROPERTIES OUTPUT_NAME "index" SUFFIX ".html")
  set_target_properties(j18314 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTDIR})


  # Emscripten向け：リンクオプションでフラグを渡す
  target_link_options(j18314 PRIVATE
    "-o" "${OUTDIR}/index.html"
    
    # 互換GL（固定機能/古いGL呼び出しをしている場合の保険）
    "-sLEGACY_GL_EMULATION=1"

    # メモリ
    "-sALLOW_MEMORY_GROWTH=1"
    "-sABORTING_MALLOC=0"

    # デバッグ/検証
    "-sASSERTIONS=2"
    "-sGL_ASSERTIONS=1"
    "-gsource-map"
    "-g"

    "--preload-file" "../images@/images"
    "--use-preload-plugins"

    # もし glutMainLoop が絡むなら必要になることが多い（必要なら有効化）
    # "-sEXIT_RUNTIME=0"
  )

  # もしアセットがあるなら（例：imagesフォルダ）—必要なければ削除OK
  # target_link_options(j18314 PRIVATE

  # )

  # GLUT を使っている場合（glut32相当）
  # 使ってないなら不要。使ってるならまずこれを試すのが無難です。
  # target_link_options(j18314 PRIVATE "-sUSE_GLUT=3")

else()
  # ---- Native (Cygwin/Windows) ----

  # アイコンrcをwindresで .o にしてリンク（元Makefile相当）
  set(ICONRC  ${CMAKE_CURRENT_SOURCE_DIR}/myicon.rc)
  set(ICONOBJ ${CMAKE_CURRENT_BINARY_DIR}/myicon.o)

  find_program(WINDRES NAMES i686-pc-cygwin-windres windres)
  if(NOT WINDRES)
    message(FATAL_ERROR "windres が見つかりません。i686-pc-cygwin-windres / windres を用意してください。")
  endif()

  add_custom_command(
    OUTPUT  ${ICONOBJ}
    COMMAND ${WINDRES} -i ${ICONRC} -o ${ICONOBJ}
    DEPENDS ${ICONRC}
    VERBATIM
  )
  add_custom_target(iconobj DEPENDS ${ICONOBJ})
  add_dependencies(j18314 iconobj)

  # 生成された .o を最終リンクに含める
  target_sources(j18314 PRIVATE ${ICONOBJ})

  # 元Makefileのライブラリ
  # Makefile: -lglpng -lglut32 -lglu32 -lopengl32 -lm
  target_link_libraries(j18314 PRIVATE
    glpng
    glut32
    glu32
    opengl32
    m
  )
endif()
