var Constructor = function()
{
    this.getCOStyles = function()
    {
        // string array containing the endings of the alternate co style
        // e.g. co_adder has ["+alt"]
        return ["+alt"];
    };

    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(2);
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        powerNameAnimation.queueAnimation(animation2);
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.mp3", 992, 45321);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3", 1505, 49515);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/tagpower.mp3", 14611, 65538);
                break;
            default:
                audio.addMusic("resources/music/cos/hachi.mp3", 12402, 68059);
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 1;
    };
    this.getCOArmy = function()
    {
        return "OS";
    };
    this.getCostModifier = function(co, id, baseCost)
    {
		switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
				return -baseCost * 0.5;
            default:
                break;
        }
        return -baseCost * 0.1;
    };

    this.getAdditionalBuildingActions = function(co, building)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (building.getBuildingID() === "TOWN" &&
                    building.getOwner() === co.getOwner())
                {
                    return "ACTION_BUILD_UNITS";
                }
                break;
            case GameEnums.PowerMode_Power:
                break;
            default:
                break;
        }
        return "";
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 10;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 10;
                }
                break;
        }
        return 0;
    };
    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
                co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
        return 0;
    };
    // CO - Intel
    this.getBio = function()
    {
        return qsTr("Owner of the Battle Maps shop. Rumored to be Orange Star's former commander-in-chief.");
    };
    this.getHits = function()
    {
        return qsTr("Tea");
    };
    this.getMiss = function()
    {
        return qsTr("Medicine");
    };
    this.getCODescription = function()
    {
        return qsTr("Uses secret trade routes to get slightly lower deployment costs for all units.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nGlobal Effect: \nUnits are cheaper") +
               qsTr("\n\nCO Zone Effect: \nUnits have more firepower and defense.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Speaks with such authority that he obtains even lower deployment costs.");
    };
    this.getPowerName = function()
    {
        return qsTr("Barter");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Merchant pals gather from around the globe and help him deploy ground units from any allied city even cheaper.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Merchant Union");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("This brings back memories!"),
                qsTr("Runnin' away won't prove anything! Stand your ground, soldier!"),
                qsTr("No need to get all worked up!"),
                qsTr("Hey, I'm no retiree!"),
                qsTr("I may be old, but I can still rumble!"),
                qsTr("This is my best seller!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Battles cost too much!"),
                qsTr("Rematches are free!"),
                qsTr("Thank you, come again!")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("I'm getting too old for this."),
                qsTr("Shop's closing for today...")];
    };
    this.getName = function()
    {
        return qsTr("Hachi");
    };
}

Constructor.prototype = CO;
var CO_HACHI = new Constructor();
