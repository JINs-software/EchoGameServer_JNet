### \[에코 서버 성능/오류 테스트\]
* 7일간의 에코 성능 테스트
  
#### 테스트 조건
* 수신 TPS: 150만 이상
* 송신 TPC: 150만 이상
* 메모리 사용량: 1G 미만
* 더미 프로그램(프로카데미 제공)
  * 더미 그룹1: 2500명 연결 유지 모드
  * 더미 그룹2: 2500명 연결 유지 모드
 
#### 테스트 결과 => 통과!
* 게임 서버 메모리: 249MB
  ![image](https://github.com/user-attachments/assets/3a9c75be-9445-4163-a9ff-3dab60891d7b)
* 수신 TPS: 1995365, 송신 TPS: 1995203
  ![image](https://github.com/user-attachments/assets/6d3ab134-9dc7-4f42-a671-88972eea9c2a)
* 더미 그룹(클라이언트) 창 (1U 2500 X 2)
  ![image](https://github.com/user-attachments/assets/6e681306-ecf3-419d-a737-b35777b93bce)

---

### \[에코 재접속 모드 테스트\]
* 7일간의 에코 재접속 모드 테스트

#### 테스트 조건
* 더미 그룹 5000명
* Accept TPS: 1000 ~ 1500 내외

#### 테스트 결과 => 통과!
* 더미 그룹(클라이언트) 중지 전
  ![image](https://github.com/user-attachments/assets/68efb319-292d-4b99-9c69-27bffbe11fef)
  ![image](https://github.com/user-attachments/assets/8e16c5d8-2d11-4e19-9e11-d8cc5bbb2a7b)
  * Accept TPS: 1179.06
* 더미 그룹(클라이언트) 중지 후
  ![image](https://github.com/user-attachments/assets/914c28de-9cd4-458c-b35c-d3f067b71253)
