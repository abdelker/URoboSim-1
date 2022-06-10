#include "Factory/RModelFactory.h"
#include "Factory/RModelBuilder.h"
#include "Physics/RModel.h"
#include "Engine/World.h"
#include "Editor/EditorEngine.h"

URModelFactory::URModelFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    // AActor subclass this ActorFactory creates.
    NewActorClassName = FString("ARModel");
    NewActorClass = ARModel::StaticClass();
    bDrag = false;
}

bool URModelFactory::CanCreateActorFrom(const FAssetData & AssetData, FText & OutErrorMsg)
{
    // Only designed for SDFData Asset.
    return AssetData.GetClass()->IsChildOf( USDFDataAsset::StaticClass());
    // return AssetData.AssetClass.IsEqual(FName("SDFDataAsset"));
}

AActor* URModelFactory::GetDefaultActor(const FAssetData & AssetData)
{
    return NewActorClass->GetDefaultObject<ARModel>();
}

#if ENGINE_MINOR_VERSION < 27 || ENGINE_MAJOR_VERSION >4

AActor* URModelFactory::SpawnActor(UObject* Asset, ULevel* InLevel, const FTransform & Transform, EObjectFlags InObjectFlags, const FName Name)
{
     USDFDataAsset* SDFAsset = CastChecked<USDFDataAsset>(Asset);
     if(SDFAsset)
       {
         ARModel* NewRobot = nullptr;
         for(USDFModel* Model : SDFAsset->Models)
           {
             AActor* DefaultActor = GetDefaultActor(FAssetData(Asset));
             if (DefaultActor)
               {
                 FActorSpawnParameters SpawnInfo;
                 SpawnInfo.OverrideLevel = InLevel;
                 SpawnInfo.ObjectFlags = InObjectFlags;
                 SpawnInfo.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;

                 //TODO fix name of spawned model
                 SpawnInfo.Name = FName(*Model->Name);
                 UE_LOG(LogTemp, Error, TEXT("Create Model %s"), *SpawnInfo.Name.ToString());

                 // Creates RRobot Actor.

                 URModelBuilder* ModelBuilder = NewObject<URModelBuilder>(this);
                 NewRobot = (ARModel*)InLevel->OwningWorld->SpawnActor(DefaultActor->GetClass(), &Transform, SpawnInfo);
                 ModelBuilder->Load(Model, NewRobot, Transform.GetLocation(),SDFAsset->Version);

                 PostSpawnActor(Asset, NewRobot);

               }
           }
         return NewRobot;
       }
     else
       {
         UE_LOG(LogTemp, Error, TEXT("Asset cast to USDFDataAsset failed"));

       }

     // Creates RRobot Actor.
     UE_LOG(LogTemp, Warning, TEXT("No default Robot Actor available\n"));
 return nullptr;

}
#else

AActor* URModelFactory::SpawnActor(UObject* InAsset, ULevel* InLevel, const FTransform& InTransform, const FActorSpawnParameters& InSpawnParams)
{
     USDFDataAsset* SDFAsset = CastChecked<USDFDataAsset>(InAsset);
     if (SDFAsset)
       {
         ARModel* NewRobot = nullptr;
         for (USDFModel* Model : SDFAsset->Models)
           {
             AActor* DefaultActor = GetDefaultActor(FAssetData(InAsset));
             if (DefaultActor)
               {
                 // Creates RRobot Actor.
                 URModelBuilder* ModelBuilder = NewObject<URModelBuilder>(this);
                 NewRobot = (ARModel*)InLevel->OwningWorld->SpawnActor(DefaultActor->GetClass(), &InTransform, InSpawnParams);
                 ModelBuilder->Load(Model, NewRobot, InTransform.GetLocation(),SDFAsset->Version);

                 PostSpawnActor(InAsset, NewRobot);
               }
           }
         return NewRobot;
       }
     else
       {
         UE_LOG(LogTemp, Error, TEXT("Asset cast to USDFDataAsset failed"));
       }

     // Creates RRobot Actor.
     UE_LOG(LogTemp, Warning, TEXT("No default Robot Actor available\n"));
 return nullptr;
}
#endif //Version

void URModelFactory::PostSpawnActor( UObject* Asset, AActor* NewActor )
{
  USDFDataAsset* SDFAsset = CastChecked<USDFDataAsset>(Asset);
  if(SDFAsset)
    {
      FActorLabelUtilities::SetActorLabelUnique(NewActor, SDFAsset->Models[0]->Name);
    }
}

void URModelFactory::PostCreateBlueprint( UObject* Asset, AActor* CDO )
{

}
