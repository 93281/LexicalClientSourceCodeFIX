#include "Offhand.h"
#include "../../../../../../Utils/Logger.h"

Offhand::Offhand() : Module("Offhand", "Automatically puts totems/shields into your offhand", Category::PLAYER) {
	itemModeEnum = (EnumSetting*)registerSetting(new EnumSetting("Item", "Item to switch ", { "Totem", "Shield" }, &itemMode, 0));
}

std::string Offhand::getModeText() {
	return itemModeEnum->enumList[itemMode];
}

void Offhand::onNormalTick(LocalPlayer* localPlayer) {
    if (localPlayer->getOffhandSlot()->item.get() != nullptr)
        return;

    uint16_t ItemID = (itemMode == 0) ? 584 : 362;

    Container* inventory = localPlayer->playerInventory->container;
    for (int i = 0; i < 36; i++) {
        ItemStack* itemStack = inventory->getItem(i);
        if (itemStack == nullptr)
            continue;

        Item* item = itemStack->item.get();
        if (item != nullptr && item->itemId == ItemID) {
            InventoryAction pickUpAction(
                i,
                itemStack,
                ItemStack::getEmptyItem(),
                ContainerID::Inventory
            );

            InventoryAction placeOffhandAction(
                0,
                ItemStack::getEmptyItem(),
                itemStack,
                ContainerID::Offhand
            );

            auto cit = std::make_unique<ComplexInventoryTransaction>();
            cit->type = ComplexInventoryTransaction::Type::NormalTransaction;
            cit->data.addAction(pickUpAction);
            cit->data.addAction(placeOffhandAction);

            InventoryTransactionPacket packet(std::move(cit));
            localPlayer->level->getPacketSender()->send(&packet);

            localPlayer->setOffhandSlot(itemStack);
            inventory->removeItem(i, 1);

            break;
        }
    }
}
