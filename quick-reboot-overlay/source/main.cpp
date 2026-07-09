#define TESLA_INIT_IMPL
#include <tesla.hpp>

class QuickRebootGui : public tsl::Gui {
public:
  QuickRebootGui() {}

  virtual tsl::elm::Element *createUI() override {
    auto frame = new tsl::elm::OverlayFrame(APP_TITLE, APP_VERSION);

    auto list = new tsl::elm::List();

    auto poweroffListItem = new tsl::elm::ListItem("Power Off");
    poweroffListItem->setClickListener([](u64 keys) {
      if (keys & HidNpadButton_A) {
        spsmShutdown(false);
      }
      return false;
    });
    list->addItem(poweroffListItem);

    auto rebootListItem = new tsl::elm::ListItem("Reboot");
    rebootListItem->setClickListener([](u64 keys) {
      if (keys & HidNpadButton_A) {
        spsmShutdown(true);
      }
      return false;
    });
    list->addItem(rebootListItem);

    frame->setContent(list);

    return frame;
  }

  virtual void update() override {}

  virtual bool handleInput(u64 keysDown, u64 keysHeld, const HidTouchState &touchPos,
                           HidAnalogStickState joyStickPosLeft,
                           HidAnalogStickState joyStickPosRight) override {
    return false;
  }
};

class QuickRebootErrorGui : public QuickRebootGui {
private:
  const char *const errorMessage;

public:
  QuickRebootErrorGui(const char *const errorMessage) : errorMessage(errorMessage) {}

  virtual tsl::elm::Element *createUI() override {
    auto frame = new tsl::elm::OverlayFrame(APP_TITLE, APP_VERSION);
    frame->setContent(
        new tsl::elm::CustomDrawer([this](tsl::gfx::Renderer *r, s32 x, s32 y, s32 w, s32 h) {
          r->drawString(errorMessage, false, x + 3, y + 15, 20, r->a(0xF22F));
        }));
    return frame;
  }
};

class QuickRebootOverlay : public tsl::Overlay {
private:
  Result spsmInitializeResult;

public:
  virtual void initServices() override { spsmInitializeResult = spsmInitialize(); }

  virtual void exitServices() override { spsmExit(); }

  virtual void onShow() override {}

  virtual void onHide() override {}

  virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
    if (R_FAILED(spsmInitializeResult)) {
      char msg[64];
      sprintf(msg, "Failed to init spsm service.\nError code: %u.", spsmInitializeResult);
      return initially<QuickRebootErrorGui>(msg);
    }

    return initially<QuickRebootGui>();
  }
};

int main(int argc, char **argv) { return tsl::loop<QuickRebootOverlay>(argc, argv); }
