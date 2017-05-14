#include "GameManager.h"

static CSceneManager gSceneManager;
static CControllManager gControllManager;

void CGameManager::Init() {
	try {
		if (!al_init()) { // Allegro 초기화
			throw kAllegroInitError;
		}

		m_display = al_create_display(kDisplayWidth, kDisplayHeight); // 디스플레이 생성
		if (!m_display) {
			throw kAllegroCreateDisplayError;
		}

		m_eventQueue = al_create_event_queue(); // 이벤트 큐 생성
		if (!m_eventQueue) {
			throw kAllegroCreateEventQueueError;
		}

		m_timer = al_create_timer(1.0 / kFps); // 타이머 생성
		if (!m_timer) {
			throw kAllegroCreateTimerError;
		}

		if (!al_install_keyboard()) { // 키보드 드라이버 설치
			throw;
		}
	}
	catch (std::string _exception) {
		std::cout << _exception << std::endl;
		return;
	}
	
	al_register_event_source(m_eventQueue, al_get_display_event_source(m_display)); // 디스플레이 이벤트 소스를 이벤트 큐에 등록
	al_register_event_source(m_eventQueue, al_get_timer_event_source(m_timer)); // 타이머 이벤트 소스를 이벤트 큐에 등록
	al_register_event_source(m_eventQueue, al_get_keyboard_event_source()); // 키보드 이벤트 소스를 이벤트 큐에 등록

	al_clear_to_color(al_map_rgb(0, 255, 0)); // 기존 화면 제거 후, 검정색으로 화면 설정 준비
	al_flip_display(); // 버퍼(검정색)에 있는 이미지 출력
	al_start_timer(m_timer); // 타이머 시작

	gSceneManager.Init(); // SceneManager 초기화
}

void CGameManager::Play() {

	while (true) {
		m_whatHappen = al_wait_for_event_until(m_eventQueue, &m_event, &m_timeout); // 이벤트 큐에서 이벤트를 꺼내서 m_event에 삽입

		if (m_whatHappen) { // 이벤트 발생 시
			if (m_event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { // 창을 닫은 경우
				al_clear_to_color(al_map_rgb(255, 0, 0));
				al_flip_display();
				al_rest(5.0);
				break;
			}

			if (m_event.type == ALLEGRO_EVENT_KEY_DOWN) { // 키를 누른 경우
				gControllManager.HandleEvent(m_event);
			}
			
			if (m_event.type == ALLEGRO_EVENT_TIMER) { // 화면 업데이트
				gSceneManager.Update();
				gSceneManager.Render();
			}
		}
	}

	al_destroy_display(m_display); // 디스플레이 제거
	al_destroy_event_queue(m_eventQueue); // 이벤트 큐 제거
}