#include "ROSCommunication/Action/Server/GripperCommandAction/GCAFeedbackPublisher.h"

DEFINE_LOG_CATEGORY_STATIC(LogGCAFeedbackPublisher, Log, All)

URGCAFeedbackPublisher::URGCAFeedbackPublisher()
{
  MessageType = TEXT("");
}

void URGCAFeedbackPublisher::Init()
{
  Super::Init();
  GripperController = Cast<UPR2GripperController>(Controller);
}

void URGCAFeedbackPublisher::Publish()
{
  // TODO: Implement
}