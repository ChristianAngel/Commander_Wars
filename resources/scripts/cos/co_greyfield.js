var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(3);
        co.setSuperpowerStars(3);
    };

    this.isBoostUnit = function(unit)
    {
        var unitId = unit.getUnitID();
        var idList = ["K_HELI", "T_HELI", "WATERPLANE"];
        if (unit.getUnitType() === GameEnums.UnitType_Naval ||
            idList.indexOf(unitId) >= 0)
        {
            return true;
        }
        return false;
    };

    this.activatePower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(GameEnums.PowerMode_Power);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            unit.refill();
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());

            if (animations.length < 5)
            {
                animation.addSprite("power8", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power8", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
        units.remove();
    };

    this.activateSuperpower = function(co, powerMode)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(powerMode);
        powerNameAnimation.queueAnimationBefore(dialogAnimation);

        var units = co.getOwner().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            unit.refill();
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());

            if (animations.length < 5)
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
        units.remove();
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power_ids_dc.mp3", 0 , 0);
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/power_ids_dc.mp3", 0 , 0);
                break;
            case GameEnums.PowerMode_Tagpower:
                audio.addMusic("resources/music/cos/bh_tagpower.mp3", 779 , 51141);
                break;
            default:
                audio.addMusic("resources/music/cos/greyfield.mp3", 3229, 64409);
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "TI";
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var boostUnit = CO_GREYFIELD.isBoostUnit(attacker);
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (boostUnit)
                {
                    return 70;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (boostUnit)
                {
                    return 20;
                }
                else
                {
                    return 10;
                }
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (boostUnit)
                    {
                        return 20;
                    }
                    return 10;
                }
                break;
        }
        return 0;
    };

    this.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var boostUnit = CO_GREYFIELD.isBoostUnit(defender);
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (boostUnit)
                {
                    return 70;
                }
                return 10;
            case GameEnums.PowerMode_Power:
                if (boostUnit)
                {
                    return 50;
                }
                return 10;
            default:
                if (co.inCORange(Qt.point(defPosX, defPosY), defender))
                {
                    if (boostUnit)
                    {
                        return 50;
                    }
                    return 10;
                }
                break;
        }
        return 0;
    };

    this.getMovementpointModifier = function(co, unit, posX, posY)
    {
        var boostUnit = CO_GREYFIELD.isBoostUnit(unit);
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                if (boostUnit)
                {
                    return 1;
                }
                return 0;
            case GameEnums.PowerMode_Power:
                return 0;
            default:
                break;
        }
        return 0;
    };

    this.getCOUnits = function(co, building)
    {
        var buildingId = building.getBuildingID();
        if (buildingId === "HARBOUR" ||
            buildingId === "TEMPORARY_HARBOUR")
        {
            return ["ZCOUNIT_MISSILE_SUB"];
        }
        return [];
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("Wants to rule as the supreme global leader. Ambitious and power-hungry.");
    };
    this.getHits = function()
    {
        return qsTr("Power");
    };
    this.getMiss = function()
    {
        return qsTr("Insubordinates");
    };
    this.getCODescription = function()
    {
        return qsTr("Copter, seaplanes and sea units have higher firepower and defense.");
    };
    this.getLongCODescription = function()
    {
        return qsTr("\nSpecial Unit:\nMissile Submarine\n\nGlobal Effect: \nNo Effects.") +
               qsTr("\n\nCO Zone Effect: \n Sea Units have 20% offensive and 50% defensive bonus.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("All units get resupplied and his copter, seaplanes and sea units have higher firepower and defense.");
    };
    this.getPowerName = function()
    {
        return qsTr("Supply Chain");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("All units get resupplied and his copter, seaplanes and sea units have higher firepower and defense and increase movement range by 1.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Naval War");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("I will brook no opposition. The enemy will be eliminated."),
                qsTr("Overwhelming forces need overwhelming supplies."),
                qsTr("Supplies are here!"),
                qsTr("Admiral Greyfield here. Attack and eleminate them.")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Disloyalty will be punished with death."),
                qsTr("Anyone who opposes me will be crushed!"),
                qsTr("The Teal Isles Army will prevail.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("Nooo... this is not the end of Teal Isles!"),
                qsTr("What? My army has been crushed.")];
    };
    this.getName = function()
    {
        return qsTr("Greyfield");
    };
}

Constructor.prototype = CO;
var CO_GREYFIELD = new Constructor();
