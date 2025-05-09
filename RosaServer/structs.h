#pragma once

#include <cstring>
#include <string>
#include <tuple>

#include "sol/sol.hpp"

static constexpr int maxNumberOfAccounts = 32768;
static constexpr int maxNumberOfPlayers = 256;
static constexpr int maxNumberOfHumans = 256;
static constexpr int maxNumberOfItemTypes = 46;
static constexpr int maxNumberOfItems = 1024;
static constexpr int maxNumberOfVehicleTypes = 17;
static constexpr int maxNumberOfVehicles = 512;
static constexpr int maxNumberOfRigidBodies = 8192;
static constexpr int maxNumberOfBonds = 16384;

using padding = uint8_t;

#define STRINGFY(a, b) a##b
#define STRINGFY2(a, b) STRINGFY(a, b)
#define PAD(size) padding STRINGFY2(unk, __LINE__)[size];

struct Player;
struct Human;
struct Vehicle;
struct Item;
struct RigidBody;
struct Bond;
struct StreetIntersection;
struct Event;
struct TrafficCar;
struct RotMatrix;
struct Building;

struct Vector {
	float x, y, z;

	const char* getClass() const { return "Vector"; }
	std::string __tostring() const;
	Vector __add(Vector* other) const;
	Vector __sub(Vector* other) const;
	bool __eq(Vector* other) const;
	Vector __mul(float scalar) const;
	Vector __mul_RotMatrix(RotMatrix* rot) const;
	Vector __div(float scalar) const;
	Vector __unm() const;
	void add(Vector* other);
	void mult(float scalar);
	void set(Vector* other);
	void cross(Vector* other);
	Vector clone() const;
	double dist(Vector* other) const;
	double distSquare(Vector* other) const;
	double length() const;
	double lengthSquare() const;
	double dot(Vector* other) const;
	std::tuple<int, int, int> getBlockPos() const;
	void normalize();
};

struct RotMatrix {
	float x1, y1, z1;
	float x2, y2, z2;
	float x3, y3, z3;

	const char* getClass() const { return "RotMatrix"; }
	std::string __tostring() const;
	RotMatrix __mul(RotMatrix* other) const;
	void set(RotMatrix* other);
	RotMatrix clone() const;
	Vector getForward() const;
	Vector getUp() const;
	Vector getRight() const;
	Vector realForward() const;
};

// 40 bytes (28)
struct EarShot {
	int active;
	int playerID;            // 04
	int humanID;             // 08
	int receivingItemID;     // 0c
	int transmittingItemID;  // 10
	int unk2;                // 14
	int unk3;                // 18
	int unk4;                // 1c
	float distance;          // 20
	float volume;            // 24

	const char* getClass() const { return "EarShot"; }
	bool getIsActive() const { return active; }
	void setIsActive(bool b) { active = b; }

	Player* getPlayer() const;
	void setPlayer(Player* player);
	Human* getHuman() const;
	void setHuman(Human* human);
	Item* getReceivingItem() const;
	void setReceivingItem(Item* item);
	Item* getTransmittingItem() const;
	void setTransmittingItem(Item* item);
};

// 188896 bytes (2E1E0)
struct Connection {
	unsigned int address;
	unsigned int port;           // 04
	int unusedConnectionFilter;  // 08
	int unk0;                    // 0C
	int roundNumber;             // 10
	int adminVisible;            // 14
	int playerID;                // 18
	int unk1;                    // 1c
	int bandwidth;               // 20
	int timeoutTime;             // 24
	PAD(0x4c - 0x24 - 4);
	int numReceivedEvents;  // 4c
	PAD(0x5c - 0x4c - 4);
	EarShot earShots[8];  // 5c
	PAD(0x19c - (0x5c + (sizeof(EarShot) * 8)));
	int spectatingHumanID;  // 19c
	PAD(0x1fda0 - 0x19c - 4);
	Vector cameraPosition;  // 1fda0
	PAD(0x2e1e0 - 0x1fda0 - sizeof(Vector))

	const char* getClass() const { return "Connection"; }
	std::string getAddress();
	bool getAdminVisible() const { return adminVisible; }
	void setAdminVisible(bool b) { adminVisible = b; }
	Player* getPlayer() const;
	void setPlayer(Player* player);
	EarShot* getEarShot(unsigned int idx);
	Human* getSpectatingHuman() const;
	Vector getCameraPosition() const { return cameraPosition; }
	bool hasReceivedEvent(Event* event) const;
};

// 112 bytes (70)
struct Account {
	int subRosaID;
	int phoneNumber;      // 04
	long long steamID;    // 08
	char name[32];        // 10
	int unk0;             // 30
	int money;            // 34
	int corporateRating;  // 38
	int criminalRating;   // 3c
	int spawnTimer;       // 40
	// in-game minutes
	int playTime;  // 44
	PAD(0x60 - 0x44 - 4);
	int banTime;  // 60
	PAD(112 - 104);

	const char* getClass() const { return "Account"; }
	std::string __tostring() const;
	int getIndex() const;
	sol::table getDataTable() const;
	char* getName() { return name; }
	std::string getSteamID() { return std::to_string(steamID); }
};

struct LineIntersectResult {
	Vector pos;
	Vector normal;    // 0c
	float fraction;   // 18
	float unk0;       // 1c
	int unk1;         // 20
	int unk2;         // 24
	int unk3;         // 28
	int unk4;         // 2c
	int vehicleFace;  // 30
	int humanBone;    // 34
	int unk6;         // 38
	int unk7;         // 3c
	int unk8;         // 40
	int unk9;         // 44
	int unk10;        // 48
	int unk11;        // 4c
	int unk12;        // 50
	int areaId;       // 54
	int blockX;       // 58
	int blockY;       // 5c
	int blockZ;       // 60
	int unk17;        // 64
	int unk18;        // 68
	int matMaybe;     // 6c
	int unk20;        // 70
	int unk21;        // 74
	int unk22;        // 78
	int unk23;        // 7c
	int unk24;        // 80
	int unk25;        // 84
	int unk26;        // 88
	int unk27;        // 8c
};

// 84 bytes (54)
struct Action {
	int type;
	int a;
	int b;
	int c;
	int d;
	char text[64];

	const char* getClass() const { return "Action"; }
};

// 72 bytes (48)
struct MenuButton {
	int id;
	char text[64];
	int unk;

	const char* getClass() const { return "MenuButton"; }
	char* getText() { return text; }
	void setText(const char* newText) {
		std::strncpy(text, newText, sizeof(text) - 1);
	}
};

// 131604 bytes (20214)
struct Voice {
	int isSilenced;
	int unk0;                        // 04
	int volumeLevel;                 // 08
	int currentFrame;                // 0c
	int unk1;                        // 10
	int frameVolumeLevels[64];       // 14
	int frameSizes[64];              // 114
	unsigned char frames[64][2048];  // 214

	const char* getClass() const { return "Voice"; }
	bool getIsSilenced() const { return isSilenced; }
	void setIsSilenced(bool b) { isSilenced = b; }
	std::string getFrame(unsigned int idx) const;
	void setFrame(unsigned int idx, std::string_view frame, int volumeLevel);
};

// 14388 bytes (3834)
struct Player {
	int active;
	char name[32];               // 04
	int unk0;                    // 24
	int unk1;                    // 28
	unsigned int subRosaID;      // 2c
	unsigned int phoneNumber;    // 30
	int isAdmin;                 // 34
	unsigned int adminAttempts;  // 38
	unsigned int accountID;      // 3C
	PAD(0x48 - 0x3C - 4);
	int isReady;          // 48
	int money;            // 4C
	int teamMoney;        // 50
	int budget;           // 54
	int corporateRating;  // 58
	int criminalRating;   // 5c
	int isGodMode;        // 60
	int unk3;
	// for buy limit
	int itemsBought;     // 68
	int vehiclesBought;  // 6c
	int withdrawnBills;  // 70
	PAD(0x84 - 0x70 - 4);
	unsigned int team;             // 84
	unsigned int teamSwitchTimer;  // 88
	int stocks;                    // 8c
	int isRoundManager;	       // 90
	int unk6;                      // 94
	int spawnTimer;  // 98
	int humanID;     // 9c
	float gearX;             // a0
	float leftRightInput;    // a4
	float gearY;             // a8
	float forwardBackInput;  // ac
	float viewYawDelta;      // b0
	float viewPitch;         // b4
	float freeLookYaw;       // b8
	float freeLookPitch;     // bc
	float viewYaw;           // c0
	PAD(0xe4 - 0xc0 - 4);
	float viewPitchDelta;  // e4
	PAD(0x120 - 0xe4 - 4);
	unsigned int inputFlags;      // 120
	unsigned int lastInputFlags;  // 124
	PAD(0x134 - 0x124 - 4);
	int zoomLevel;  // 134
	PAD(0x158 - 0x134 - 4);
	// 0 = none, 1 = human, 2 = in car, 3 = in helicopter
	int inputType;  // 158
	int lastInputType;  // 15C
	PAD(0x164 - 0x15c - 4);
	// 0 = none, 1-19 = shop, 2X = base
	int menuTab;  // 164
	int menuTabBuildingID; // 168
	PAD(0x1b4 - 0x168 - 4);
	int numActions;      // 1b4
	int lastNumActions;  // 1b8
	PAD(0x1c8 - 0x1b8 - 4);
	Action actions[64];  // 1c8
	PAD(0x1b14 - (0x1c8 + (sizeof(Action) * 64)));
	int numMenuButtons;          // 1b14
	MenuButton menuButtons[32];  // 1b18
	PAD(0x2d18 - (0x1b18 + (sizeof(MenuButton) * 32)));
	int isBot;     // 2d18
	int isZombie;  // 2d1c
	PAD(0x2d38 - 0x2d1c - 4);
	int botHasDestination;  // 2d38
	Vector botDestination;  // 2d3c
	PAD(0x37ac - 0x2d3c - 12);
	int gender;     // 37ac
	int skinColor;  // 37b0
	int hairColor;  // 37b4
	int hair;       // 37b8
	int eyeColor;   // 37bc
	// 0 = casual, 1 = suit
	int model;      // 37c0
	int suitColor;  // 37c4
	// 0 = no tie
	int tieColor;  // 37c8
	int unk19;     // 37cc
	int head;      // 37d0
	int necklace;  // 37d4
	PAD(0x3834 - 0x37d4 - 4);

	const char* getClass() const { return "Player"; }
	std::string __tostring() const;
	int getIndex() const;
	bool getIsActive() const { return active; }
	void setIsActive(bool b) { active = b; }
	sol::table getDataTable() const;
	char* getName() { return name; }
	void setName(const char* newName) {
		std::strncpy(name, newName, sizeof(name) - 1);
	}
	bool getIsAdmin() const { return isAdmin; }
	void setIsAdmin(bool b) { isAdmin = b; }
	bool getIsReady() const { return isReady; }
	void setIsReady(bool b) { isReady = b; }
	bool getIsRoundManager() const { return isRoundManager; }
	void setIsRoundManager(bool b) { isRoundManager = b; }
	bool getIsGodMode() const { return isGodMode; }
	void setIsGodMode(bool b) { isGodMode = b; }
	bool getIsBot() const { return isBot; }
	void setIsBot(bool b) { isBot = b; }
	bool getIsZombie() const { return isZombie; }
	void setIsZombie(bool b) { isZombie = b; }
	Human* getHuman() const;
	void setHuman(Human* human);
	Connection* getConnection();
	Account* getAccount();
	void setAccount(Account* account);
	Voice* getVoice() const;
	const Vector* getBotDestination() const;
	void setBotDestination(Vector* vec);
	Action* getAction(unsigned int idx);
	MenuButton* getMenuButton(unsigned int idx);
	Building* getMenuTabBuilding() const;
	void setMenuTabBuilding(Building* building);

	Event* update() const;
	Event* updateFinance() const;
	Event* updateElimState(int trackerVisible, int playerTeam,
	                       sol::optional<Player*> savior,
	                       sol::optional<Vector*> saviorPos) const;
	void remove() const;
	void sendMessage(const char* message) const;
};

// 312 bytes (138)
struct Bone {
	int bodyID;
	Vector pos;     // 04
	Vector pos2;    // 10
	Vector vel;     // 1c
	int unk0;       // 28
	int unk1;       // 2c
	int unk2;       // 30
	RotMatrix rot;  // 34
	PAD(0x84 - 0x34 - sizeof(RotMatrix));
	float mass;              // 84
	Vector scaleReciprocal;  // 88
	Vector scale;            // 94
	PAD(0x138 - 0x94 - sizeof(Vector));

	const char* getClass() const { return "Bone"; }
};

// 40 bytes (28)
struct InventorySlot {
	int count;
	int primaryItemID;
	int secondaryItemID;
	PAD(0x1c);

	const char* getClass() const { return "InventorySlot"; }
	Item* getPrimaryItem() const;
	Item* getSecondaryItem() const;
};

// 28664 bytes (6FF8)
struct Human {
	int active;
	int physicsSim;           // 04
	int playerID;             // 08
	int accountID;            // 0c
	int unk1;                 // 10
	int unk2;                 // 14
	int burgerEatCooldown;    // 18
	int stamina;              // 1c
	int maxStamina;           // 20
	int unk4;                 // 24
	int vehicleID;            // 28
	int vehicleSeat;          // 2c
	int lastVehicleID;        // 30
	int lastVehicleCooldown;  // 34
	// counts down after death
	unsigned int despawnTime;  // 38
	int oldHealth;             // 3c
	// eliminator
	int isImmortal;                // 40
	int unk10;                     // 44
	int unk11;                     // 48
	int vehicleExitAnimTimer;      // 4c
	unsigned int spawnProtection;  // 50
	int isOnGround;                // 54
	/*
	0=normal
	1=jumping/falling
	2=sliding
	5=getting up?
	*/
	int movementState;        // 58
	int standingUpAnimTimer;  // 5c
	int zoomLevel;            // 60
	int unk14;                // 64
	int unk15;                // 68
	int unk16;                // 6c
	float throwPitch;         // 70
	int unk18;                // 74
	// max 60
	int damage;       // 78
	int isStanding;   // 7c
	Vector pos;       // 80
	Vector pos2;      // 8c
	float viewYaw;    // 98
	float viewPitch;  // 9c
	PAD(0xd8 - 0x9c - 4);
	float viewYaw2;  // d8
	PAD(0x118 - 0xd8 - 4);
	float crouchHeight; // 118
	PAD(0x128 - 0x118 - 4);
	float gearX;        // 128
	float strafeInput;  // 12c
	float gearY;        // 130
	float walkInput;    // 134
	float viewYawVel;   // 138
	float viewPitch2;   // 13c
	PAD(0x214 - 0x13c - 4);
	/*
	mouse1 = 1		1 << 0
	mouse2 = 2		1 << 1
	space = 4		1 << 2
	ctrl = 8		1 << 3
	shift = 16		1 << 4

	Q = 32			1 << 5
	e = 2048		1 << 11
	r = 4096		1 << 12
	f = 8192		1 << 13

	del = 262144	1 << 18
	z = 524288		1 << 19
	*/
	unsigned int inputFlags;      // 214
	unsigned int lastInputFlags;  // 218
	PAD(0x220 - 0x218 - 4);
	Bone bones[16];  // 220
	PAD(0x6ad0 - (0x220 + (sizeof(Bone) * 16)));
	InventorySlot inventorySlots[6];  // 6ad0
	PAD(0x6d50 - (0x6ad0 + (sizeof(InventorySlot) * 6)));
	int health;      // 6d50
	int bloodLevel;  // 6d54
	int isBleeding;  // 6d58
	int chestHP;     // 6d5c
	int unk27;       // 6d60
	int headHP;      // 6d64
	int unk28;       // 6d68
	int leftArmHP;   // 6d6c
	int unk29;       // 6d70
	int rightArmHP;  // 6d74
	int unk30;       // 6d78
	int leftLegHP;   // 6d7c
	int unk31;       // 6d80
	int rightLegHP;  // 6d84
	PAD(0x6ddc - 0x6d84 - 4);
	int progressBar;                        // 6ddc
	int inventoryAnimationFlags;            // 6de0
	float inventoryAnimationProgress;       // 6de4
	int inventoryAnimationDuration;         // 6de8
	int inventoryAnimationHand;             // 6dec
	int inventoryAnimationSlot;             // 6df0
	int inventoryAnimationCounterFinished;  // 6df4
	int inventoryAnimationCounter;          // 6df8
	PAD(0x6f80 - 0x6df8 - 4);
	int gender;                  // 6f80
	int head;                    // 6f84
	int skinColor;               // 6f88
	int hairColor;               // 6f8c
	int hair;                    // 6f90
	int eyeColor;                // 6f94
	int model;                   // 6f98
	int suitColor;               // 6f9c
	int tieColor;                // 6fa0
	int unk34;                   // 6fa4
	int necklace;                // 6fa8
	int lastUpdatedWantedGroup;  // 6fac
	PAD(0x6FF8 - 0x6fac - 4);

	const char* getClass() const { return "Human"; }
	std::string __tostring() const;
	int getIndex() const;
	bool getIsActive() const { return active; }
	void setIsActive(bool b) { active = b; }
	sol::table getDataTable() const;
	bool getIsAlive() const { return oldHealth > 0; }
	void setIsAlive(bool b) { oldHealth = b ? 100 : 0; }
	bool getIsImmortal() const { return isImmortal; }
	void setIsImmortal(bool b) { isImmortal = b; }
	bool getIsOnGround() const { return isOnGround; }
	bool getIsStanding() const { return isStanding; }
	bool getIsBleeding() const { return isBleeding; }
	void setIsBleeding(bool b) { isBleeding = b; }
	Player* getPlayer() const;
	void setPlayer(Player* player);
	Account* getAccount();
	void setAccount(Account* account);
	Vehicle* getVehicle() const;
	void setVehicle(Vehicle* vehicle);
	Bone* getBone(unsigned int idx);
	RigidBody* getRigidBody(unsigned int idx) const;
	InventorySlot* getInventorySlot(unsigned int idx);
	Human* getRightHandGrab() const;
	void setRightHandGrab(Human* man);
	Human* getLeftHandGrab() const;
	void setLeftHandGrab(Human* man);

	void remove() const;
	void teleport(Vector* vec);
	void speak(const char* message, int distance) const;
	void arm(int weapon, int magCount) const;
	void setVelocity(Vector* vel) const;
	void addVelocity(Vector* vel) const;
	bool mountItem(Item* childItem, unsigned int slot) const;
	void applyDamage(int bone, int damage) const;
};

// 5072 bytes (13D0)
struct ItemType {
	int unk0;
	int price;           // 04
	float mass;          // 08
	int unk1;            // 0c
	int isGun;           // 10
	int messedUpAiming;  // 14
	// in ticks per bullet
	int fireRate;  // 18
	//?
	int bulletType;        // 1c
	int unk2;              // 20
	int magazineAmmo;      // 24
	float bulletVelocity;  // 28
	float bulletSpread;    // 2c
	char name[64];         // 30
	PAD(0x7c - 0x30 - 64);
	int numHands;         // 7c
	Vector rightHandPos;  // 80
	Vector leftHandPos;   // 8c
	PAD(0xb0 - 0x8c - 12);
	float primaryGripStiffness;  // b0
	PAD(0xbc - 0xb0 - 4);
	float primaryGripRotation;     // bc
	float secondaryGripStiffness;  // c0
	PAD(0xcc - 0xc0 - 4);
	float secondaryGripRotation;  // cc
	PAD(0x104 - 0xcc - 4);
	Vector boundsCenter;  // 104
	PAD(0x11c - 0x104 - 12);
	int canMountTo[maxNumberOfItemTypes];  // 11c
	PAD(0x1394 - 0x11c - (4 * maxNumberOfItemTypes));
	Vector gunHoldingPos;  // 1394
	PAD(0x13D0 - 0x1394 - 12);

	const char* getClass() const { return "ItemType"; }
	std::string __tostring() const;
	int getIndex() const;
	char* getName() { return name; }
	void setName(const char* newName) {
		std::strncpy(name, newName, sizeof(name) - 1);
	}
	bool getIsGun() const { return isGun; }
	void setIsGun(bool b) { isGun = b; }

	bool getCanMountTo(ItemType* parent) const;
	void setCanMountTo(ItemType* parent, bool b);
};

// 7040 bytes (1B80)
struct Item {
	int active;
	int physicsSim;      // 04
	int physicsSettled;  // 08
	// counts to 60 ticks before settling
	int physicsSettledTimer;  // 0c
	int isStatic;             // 10
	int type;                 // 14
	float mass;                 // 18
	int despawnTime;          // 1c
	int grenadePrimerID;      // 20
	int parentHumanID;        // 24
	int parentItemID;         // 28
	int parentSlot;           // 2c
	int isInPocket;           // 30
	int numChildItems;        // 34
	int childItemIDs[4];      // 38
	PAD(0x58 - 0x38 - 16);
	int bodyID;     // 58
	Vector pos;     // 5c
	Vector pos2;    // 68
	Vector vel;     // 74
	Vector vel2;    // 80
	Vector vel3;    // 8c
	Vector vel4;    // 98
	RotMatrix rot;  // a4
	PAD(0x104 - 0xa4 - 36);
	Vector boundingBoxCornerA; // 104
	Vector boundingBoxCornerB; // 110
	PAD(0x138 - 0x110 - 12);
	int health;  // 138
	int cooldown;  // 13C
	int unk3;      // 140
	int bullets;   // 144
	PAD(0x150 - 0x144 - 4);
	int inputFlags;    // 150
	int lastInputFlags;    // 154
	PAD(0x15C - 0x154 - 4);
	int connectedPhoneID;    // 15C
	int phoneNumber;         // 160
	int callerRingTimer;     // 164
	int displayPhoneNumber;  // 168
	int enteredPhoneNumber;  // 16C
	PAD(0x278 - 0x16C - 4);
	int phoneTexture;  // 278
	int phoneStatus;   // 27C
	int vehicleID;     // 280
	PAD(0x2a0 - 0x280 - 4);
	int cashSpread;      // 2A0
	int cashBillAmount;  // 2A4
	int cashPureValue;   // 2A8
	PAD(0x368 - 0x2a8 - 4);
	union {
		unsigned int computerCurrentLine;  // 368
		unsigned int memoText;             // 368
		int team;             		   // 368
	};
	unsigned int computerTopLine;  // 36c
	//-1 for no cursor
	int computerCursor;          // 370
	char computerLines[32][64];  // 374
	PAD(0xb74 - 0x374 - (64 * 32));
	unsigned char computerLineColors[32][64];  // b74
	PAD(0x1658 - 0xb74 - (64 * 32));
	int computerTeam;   // 1658
	int computerState;  // 165c
	PAD(0x1664 - 0x165c - 4);
	int computerMissionConnectTimer;  // 1664
	int computerMissionSubMenu;       // 1668
	PAD(0x1b80 - 0x1668 - 4);

	const char* getClass() const { return "Item"; }
	std::string __tostring() const;
	int getIndex() const;
	bool getIsActive() const { return active; }
	void setIsActive(bool b) { active = b; }
	sol::table getDataTable() const;
	bool getHasPhysics() const { return physicsSim; }
	void setHasPhysics(bool b) { physicsSim = b; }
	bool getPhysicsSettled() const { return physicsSettled; }
	void setPhysicsSettled(bool b) { physicsSettled = b; }
	bool getIsStatic() const { return isStatic; }
	void setIsStatic(bool b) { isStatic = b; }
	bool getIsInPocket() const { return isInPocket; }
	void setIsInPocket(bool b) { isInPocket = b; }
	ItemType* getType();
	void setType(ItemType* itemType);
	char* getMemoText() { return (char*)&memoText; }

	void remove() const;
	Player* getGrenadePrimer() const;
	void setGrenadePrimer(Player* player);
	Human* getParentHuman() const;
	void setParentHuman(Human* human);
	Item* getParentItem() const;
	void setParentItem(Item* item);
	RigidBody* getRigidBody() const;
	Item* getChildItem(unsigned int idx) const;
	Item* getConnectedPhone() const;
	void setConnectedPhone(Item* item);
	Vehicle* getVehicle() const;
	void setVehicle(Vehicle* vehicle);
	bool mountItem(Item* childItem, unsigned int slot) const;
	bool unmount() const;
	Event* update() const;
	void speak(const char* message, int distance) const;
	void explode() const;
	void sound(int soundType, float volume, float pitch) const;
	void soundSimple(int soundType) const;
	void setMemo(const char* memo) const;
	void computerTransmitLine(unsigned int line) const;
	void computerIncrementLine() const;
	void computerSetLine(unsigned int line, const char* newLine);
	void computerSetLineColors(unsigned int line, std::string_view colors);
	void computerSetColor(unsigned int line, unsigned int column,
	                      unsigned char color);
	void cashAddBill(int position, int value) const;
	void cashRemoveBill(int position) const;
	int cashGetBillValue() const;
};

// 99776 bytes (185C0)
struct VehicleType {
	int usesExternalModel;
	int unk0;               // 04
	int controllableState;  // 08
	PAD(0x14 - 0x08 - 4);
	char name[32];  // 14
	int price;      // 34
	float mass;     // 38
	PAD(0x17874 - 0x38 - 4);
	float acceleration;  // 17874
	int numWheels;       // 17878
	PAD(0x18534 - 0x17878 - 4);
	Vector carBodyOffset;  // 18534
	PAD(0x185C0 - 0x18534 - sizeof(Vector));

	const char* getClass() const { return "VehicleType"; }
	std::string __tostring() const;
	int getIndex() const;
	bool getUsesExternalModel() const { return usesExternalModel; }
	char* getName() { return name; }
	void setName(const char* newName) {
		std::strncpy(name, newName, sizeof(name) - 1);
	}
};

// 172 bytes (AC)
struct Wheel {
	int bodyID;  // 00
        int unk0;    // 04
	int isPopped; // 08
	PAD(0x10 - 0x08 - 4);
	int health;  // 10
	PAD(0x30 - 0x10 - 4);
	float mass;  // 30
	float size;  // 34
	PAD(0x40 - 0x34 -  4);
	// nothing 					// 08 to 20
	// weird 					// 24
	// height?					// 28
	// weird					// 2C
	// weight? traction?		// 30
	// spin and height			// 34
	// nothing 					// 38 to 3C
	float spin;  // 40
	// nothing 					// 44 to 6C
	PAD(0x70 - 0x40 - 4);
	float visualHeight;  // 70
	// wheel pos? bounce? 		// 74
	// suspension height? 		// 78
	// weird			 		// 7C
	PAD(0x80 - 0x70 - 4);
	float vehicleHeight;  // 80
	float skid;           // 84
	// nothing, 136 to 171
	PAD(0xac - 0x84 - 4);

	RigidBody* getRigidBody();
	bool getIsPopped() const { return isPopped; }
	void setIsPopped(bool b) { isPopped = b; }
	const char* getClass() const { return "Wheel"; }
};

// 20840 bytes (5168)
struct Vehicle {
	int active;
	unsigned int type;      // 04
	int controllableState;  // 08
	// default 100
	int health;              // 0c
	int unk1;                // 10
	int lastDriverPlayerID;  // 14
	unsigned int color;      // 18
	//-1 = won't despawn
	short despawnTime;   // 1c
	short spawnedState;  // 1e
	int isLocked;        // 20
	int unk3;            // 24
	int bodyID;          // 28
	Vector pos;          // 2c
	Vector pos2;         // 38
	RotMatrix rot;       // 44
	int unk4;            // 68
	Vector vel;          // 6c
	PAD(0x27fc - 0x6c - 12);
	int windowStates[8];  // 27fc
	PAD(0x3600 - 0x27fc - (4 * 8));
	float gearX;         // 3600
	float steerControl;  // 3604
	float gearY;         // 3608
	float gasControl;    // 360c
	PAD(0x3648 - 0x360c - 4);
	int trafficCarID;  // 3648
	PAD(0x38a0 - 0x3648 - 4);
	float acceleration;  // 38a0
	PAD(0x3930 - 0x38a0 - 4);
	int engineRPM;  // 3930
	PAD(0x3940 - 0x3930 - 4);
	int numWheels;    // 3940
	Wheel wheels[6];  // 3944
	PAD(0x4fa8 - (0x3944 + (sizeof(Wheel) * 6)));
	int bladeBodyID;  // 4fa8
	PAD(0x50dc - 0x4fa8 - 4);
	int numSeats;  // 50dc
	PAD(0x5168 - 0x50dc - 4);

	const char* getClass() const { return "Vehicle"; }
	std::string __tostring() const;
	int getIndex() const;
	bool getIsActive() const { return active; }
	void setIsActive(bool b) { active = b; }
	VehicleType* getType();
	void setType(VehicleType* vehicleType);
	bool getIsLocked() const { return isLocked; }
	void setIsLocked(bool b) { isLocked = b; }
	sol::table getDataTable() const;
	Player* getLastDriver() const;
	RigidBody* getRigidBody() const;
	TrafficCar* getTrafficCar() const;
	void setTrafficCar(TrafficCar* trafficCar);

	Event* updateType() const;
	Event* updateDestruction(int updateType, int partID, Vector* pos,
	                         Vector* normal) const;
	void remove() const;
	bool getIsWindowBroken(unsigned int idx) const;
	void setIsWindowBroken(unsigned int idx, bool b);
	Wheel* getWheel(unsigned int idx);
};

// 92 bytes (5C)
struct Bullet {
	unsigned int type;
	int time;        // 04
	int playerID;    // 08
	float unk0;      // 0c
	float unk1;      // 10
	Vector lastPos;  // 14
	Vector pos;      // 20
	Vector vel;      // 2c
	PAD(92 - 56);

	const char* getClass() const { return "Bullet"; }
	Player* getPlayer() const;
};

// 188 bytes (BC)
struct RigidBody {
	int active;
	/*
	0 = human bone
	1 = car body
	2 = wheel
	3 = item
	*/
	int type;                   // 04
	int settled;                // 08
	int linkedHumanOrItemID;    // 0c
	int linkedBoneID;           // 10
	float mass;                 // 14
	Vector pos;                 // 18
	Vector vel;                 // 24
	Vector startVel;            // 30
	RotMatrix rot;              // 3c
	RotMatrix rotVel;           // 60
	Vector scale;               // 84
	Vector scaleReciprocal;     // 90
	float smallestScaleFactor;  // 9c
	PAD(0xBC - 0x9C - sizeof(float));

	const char* getClass() const { return "RigidBody"; }
	std::string __tostring() const;
	int getIndex() const;
	bool getIsActive() const { return active; }
	void setIsActive(bool b) { active = b; }
	sol::table getDataTable() const;
	bool getIsSettled() const { return settled; }
	void setIsSettled(bool b) { settled = b; }
	Bond* bondTo(RigidBody* other, Vector* thisLocalPos,
	             Vector* otherLocalPos) const;
	Bond* bondRotTo(RigidBody* other) const;
	Bond* bondToLevel(Vector* localPos, Vector* globalPos) const;
	void collideLevel(Vector* localPos, Vector* normal, float, float, float,
	                  float) const;
};

// 244 bytes (F4)
struct Bond {
	int active;
	/*
	 * 4 = rigidbody_level
	 * 7 = rigidbody_rigidbody
	 * 8 = rigidbody_rot_rigidbody
	 */
	int type;         // 04
	int unk0;         // 08
	int despawnTime;  // 0c
	PAD(0x2c - 0x0c - 4);
	// for level bonds
	Vector globalPos;  // 2c
	Vector localPos;   // 38
	// for non-level bonds
	Vector otherLocalPos;  // 44
	PAD(0x98 - 0x44 - 12);
	int bodyID;       // 98
	int otherBodyID;  // 9c
	PAD(0xF4 - 0x9c - 4);

	const char* getClass() const { return "Bond"; }
	std::string __tostring() const;
	int getIndex() const;
	void remove() const;
	bool getIsActive() const { return active; }
	void setIsActive(bool b) { active = b; }
	RigidBody* getBody() const;
	RigidBody* getOtherBody() const;
};

// 28 bytes (1C)
struct StreetLane {
	int direction;
	Vector posA;
	Vector posB;

	const char* getClass() const { return "StreetLane"; }
};

// 1584 bytes (630)
struct Street {
	char name[32];
	int unk0;               // 20
	int intersectionA;      // 24
	int intersectionB;      // 28
	int unk1[3];            // 2c
	int numLanes;           // 38
	StreetLane lanes[16];   // 3c
	float unk2[6];          // 1fc
	Vector trafficCuboidA;  // 214
	Vector trafficCuboidB;  // 220
	int numTraffic;         // 22c
	PAD(0x630 - 0x22c - 4);

	const char* getClass() const { return "Street"; }
	std::string __tostring() const;
	int getIndex() const;
	char* getName() { return name; }
	StreetIntersection* getIntersectionA() const;
	StreetIntersection* getIntersectionB() const;
	int getNumLanes() { return numLanes; }

	StreetLane* getLane(unsigned int idx);
};

// 136 bytes (88)
struct StreetIntersection {
	int unk0[3];
	Vector pos;       // 0c
	int streetEast;   // 18
	int streetSouth;  // 2c
	int streetWest;   // 20
	int streetNorth;  // 24
	PAD(0x44 - 0x24 - 4);
	int lightsState;     // 44
	int lightsTimer;     // 48
	int lightsTimerMax;  // 4c
	int lightEast;       // 50
	int lightSouth;      // 54
	int lightWest;       // 58
	int lightNorth;      // 5c
	PAD(0x88 - 0x5c - 4);

	const char* getClass() const { return "StreetIntersection"; }
	std::string __tostring() const;
	int getIndex() const;
	Street* getStreetEast() const;
	Street* getStreetSouth() const;
	Street* getStreetWest() const;
	Street* getStreetNorth() const;
};

// 1532 bytes (5fc)
struct TrafficCar {
	int type;       // 00
	int humanID;    // 04
	int vehicleID;  // 08
	int state;      // 0c
	Vector pos;     // 10
	Vector vel;     // 1c
	float yaw;      // 28
	RotMatrix rot;  // 2c
	PAD(0x7c - 0x2c - sizeof(RotMatrix));
	int isBot;         // 7c
	int isAggressive;  // 80
	PAD(0x5d8 - 0x80 - 4);
	int color;  // 5d8
	PAD(0x5fc - 0x5d8 - 4);

	const char* getClass() const { return "TrafficCar"; }
	std::string __tostring() const;
	int getIndex() const;
	VehicleType* getType();
	void setType(VehicleType* vehicleType);
	Human* getHuman() const;
	void setHuman(Human* human);
	Vehicle* getVehicle() const;
	void setVehicle(Vehicle* vehicle);
	bool getIsBot() const { return isBot; };
	void setIsBot(bool b) { isBot = b; };
	bool getIsAggressive() const { return isAggressive; };
	void setIsAggressive(bool b) { isAggressive = b; };
};

// 12 bytes (C)
struct ShopCar {
	int type;
	int price;
	int color;

	const char* getClass() const { return "ShopCar"; }
	VehicleType* getType();
	void setType(VehicleType* vehicleType);
};

// 56076 bytes (DB0C)
struct Building {
	int type;                // 00
	int unk0[3];             // 04
	Vector pos;              // 10
	RotMatrix spawnRot;      // 1c
	Vector interiorCuboidA;  // 40
	Vector interiorCuboidB;  // 4C
	PAD(0xC9F4 - 0x4c - 12);
	int numShopCars;       // C9F4
	ShopCar shopCars[16];  // C9F8
	int shopCarSales;      // CAB8
	PAD(0xDB0C - 0xCAB8 - 4);

	const char* getClass() const { return "Building"; }
	std::string __tostring() const;
	int getIndex() const;
	ShopCar* getShopCar(unsigned int idx);
};

// 128 bytes (80)
struct Event {
	int type;          // 00
	int tickCreated;   // 04
	Vector vectorA;    // 08
	Vector vectorB;    // 14
	int a;             // 20
	int b;             // 24
	int c;             // 28
	int d;             // 2c
	float floatA;      // 30
	float floatB;      // 34
	int unk0;          // 38
	int unk1;          // 3c
	char message[64];  // 40

	const char* getClass() const { return "Event"; }
	std::string __tostring() const;
	int getIndex() const;
	char* getMessage() { return message; }
	void setMessage(const char* newMessage) {
		std::strncpy(message, newMessage, sizeof(message) - 1);
	}
};

// 1116 bytes (45C)
struct Mission {
	int active;  // 00
	PAD(0x0c - 4);
	int type;    // 0c
	int itemID;  // 10
	PAD(0x1c - 0x10 - 4);
	int team1ID;  // 1c
	int team2ID;  // 20
	PAD(0x3c - 0x20 - 4);
	int diskTypeID;        // 3c
	int value;             // 40
	unsigned int sunTime;  // 44
	int location;          // 48
	int providedCash;      // 4c
	PAD(0x45c - 0x4c - 4);

	bool getIsActive() const { return active; }
	void setIsActive(bool b) { active = b; }
	ItemType* getDiskType() const;
	void setDiskType(ItemType* itemType);
	Item* getItem();
	void setItem(Item* item);
};

// 23492 bytes (5BC4)
struct Corporation {
	Vector interiorCuboidA;  // 00
	Vector interiorCuboidB;  // 0C
	Vector vaultCuboidA;     // 18
	Vector vaultCuboidB;     // 24
	float tableOrientation;  // 30
	Vector tableLocation;    // 34
	Vector spawnLocation;    // 40
	PAD(0x54 - 0x40 - 12);
	int players;  // 54
	PAD(0x60 - 0x54 - 4);
	int isDoorOpen;       // 60
	int managerPlayerID;  // 64
	PAD(0x17c - 0x64 - 4);
	Vector doorPos;  // 17c
	PAD(0x19c - 0x17c - 12);
	Mission missions[16];  // 19c
	PAD(0x4bc0 - 0x19c - sizeof(Mission) * 16);
	Vector carSpawn1;  // 4bc0
	RotMatrix unk0;    // 4bcc
	PAD(0x5BC4 - 0x4bcc - 36);

	const char* getClass() const { return "Corporation"; }
	int getIndex() const;
	Mission* getMission(unsigned int idx);
	void updateMission(unsigned int idx);
};
