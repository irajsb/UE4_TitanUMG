// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatComponent.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "Engine/DemoNetDriver.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UChatComponent::UChatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	
}


// Called when the game starts
void UChatComponent::BeginPlay()
{
	Super::BeginPlay();

APlayerState* PlayerState=	Cast<APlayerState>(GetOwner());
	if(GetOwnerRole()==ROLE_Authority)
	{
		SetPlayerName(	PlayerState->GetPlayerName());
	
	}
	// ...
	
}

void UChatComponent::SendString(uint8 TeamIndex,const FString& Input)
{
	if(Input.IsEmpty())
		return;
	//TODO You May Need to log Messages on Server for player reports

	UE_LOG(LogTemp,Log,TEXT("Chat Log %s"),*Input);
	if(PlayerName==TEXT("None"))
	{
		UE_LOG(LogTemp,Error,TEXT(" No Player Name"));
		if(RejectMessagesWithNoPlayerName)
		return;
	}
	
	if(GetOwnerRole()<ROLE_Authority)
	{
		SendStringOnServer(TeamIndex,Input);
	}else
	{
		

		TSubclassOf<APlayerState> Class=APlayerState::StaticClass();
		for(TActorIterator<APlayerState> It(GetWorld(), Class); It; ++It)
		{

		UChatComponent* ChatComponent=GetChatComponent(*It);
		if(ChatComponent)
		{
			ChatComponent->NotifyMessageRecived(TeamIndex,Input);
			UE_LOG(LogTemp,Log,TEXT("Message Notified"));
		}
		}
	
		
	}
}

void UChatComponent::SetPlayerName(const FString& Input)
{
	if(GetOwnerRole()<ROLE_Authority)
	{
	SetPlayerNameOnServer(Input);
	}else
	{
		PlayerName=Input;
	}
}

void UChatComponent::NotifyMessageRecived(uint8 TeamIndex, const FString& Input)
{
	if(GetOwnerRole()==ROLE_Authority)
	{
		NotifyMessageRecivedOnClinet(TeamIndex,Input);
		
	}
	OnReciveMessage.Broadcast(PlayerName,TeamIndex,MyTeamIndex,Input);
}

void UChatComponent::NotifyMessageRecivedOnClinet_Implementation(uint8 TeamIndex, const FString& Input)
{
	if(GetOwnerRole()<ROLE_Authority)
	NotifyMessageRecived(TeamIndex,Input);
}

UChatComponent* UChatComponent::GetChatComponent(APlayerState* PlayerState)
{
return Cast<UChatComponent>(PlayerState->GetComponentByClass(UChatComponent::StaticClass()));
}

void UChatComponent::SetPlayerNameOnServer_Implementation(const FString& Input)
{
	SetPlayerName(Input);
}


void UChatComponent::SetTeamIndexOnServer_Implementation(uint8 Index)
{
	MyTeamIndex=Index;
}



void UChatComponent::SendStringOnServer_Implementation(uint8 TeamIndex,const FString& Input)
{
	SendString(TeamIndex, Input);
}


void UChatComponent::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UChatComponent,PlayerName);
	DOREPLIFETIME(UChatComponent,MyTeamIndex);
}