// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChatComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnReciveMessage,const FString& ,PlayerName,uint8,SenderTeamIndex,uint8,MyTeamIndex,const FString&,Message);

/*This Class Needs to be attached to a player state*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TITANUMG_API UChatComponent : public UActorComponent
{
	GENERATED_BODY()

	public:	
	// Sets default values for this component's properties
	UChatComponent();

	protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	public:	
	


	
	
	/*
	*1-254 Team Index
	*255 =No Team */

	UPROPERTY(EditAnywhere)
	bool RejectMessagesWithNoPlayerName;
	UFUNCTION(BlueprintCallable,Server,Unreliable)
    void SetTeamIndexOnServer(uint8 Index);

//0=all >0 = team
	UFUNCTION(BlueprintCallable)
    void SendString(uint8 TeamIndex,const FString& Input);
	UFUNCTION(Server,Unreliable)
    void SendStringOnServer(uint8 TeamIndex,const FString& Input);

	UFUNCTION(Server,Unreliable)
	void SetPlayerNameOnServer(const FString& Input);
	/*Safer to be called from server*/
	UFUNCTION(BlueprintCallable)
	void SetPlayerName(const FString& Input);


	void NotifyMessageRecived(uint8 TeamIndex,const FString& Input);
	UFUNCTION(Client,Unreliable)
    void NotifyMessageRecivedOnClinet(uint8 TeamIndex,const FString& Input);




	UPROPERTY(BlueprintAssignable)
	FOnReciveMessage OnReciveMessage;

	
	private:
	UPROPERTY(Replicated)
	uint8 MyTeamIndex;
	
	UPROPERTY(Replicated)
	FString PlayerName=TEXT("None");
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static UChatComponent* GetChatComponent(APlayerState* PlayerState);
};


