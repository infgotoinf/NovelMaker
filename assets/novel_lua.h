#include <string>
const std::string novel_lua_text_data = R"lua(
-- Template novel project

local root = "./"
local images = root .. "images/"
local backgrounds_dir = images .. "backgrounds/"
local characters_dir = images .. "characters/"

local images = {
  backgrounds = {
    dir = backgrounds_dir,
    forest = "forest.jpg",
    winter_mountain = "winter_mountain.jpg",
    danils_home1 = "danils_home1.png",
    danils_home2 = "danils_home2.png",
    black_screen = "black_screen.png",
  },
  
  characters = {
    shark_guy = {
      dir = characters_dir .. "shark_guy/",
      TwT = "TwT.png",
      angry = "angry.png",
      grinning = "grinning.png",
      scarry = "scarry.png",
    },
    danil = {
      dir = characters_dir .. "danil/",
      agree = "agree.png",
      disagree = "disagree.png",
      exited = "exited.png",
      looking_up = "looking_up.png",
      sleeping = "sleeping.png",
      waking_up = "waking_up.png",
    },
    linus_torvalds = {
      dir = characters_dir .. "linus_torvalds/",
      based = "based.png",
      scarry = "scarry.png",
    }
  }
}

local function toPath(final_table, key)
  return final_table.dir .. final_table[key]
end


-- Novel itself
local shark_guy = images.characters.shark_guy
local danil = images.characters.danil
local linus_torvalds = images.characters.linus_torvalds

local novel = {
  {
    background = toPath(images.backgrounds, "forest"),
    {
      {
        characters = {},
        text = "...Это было где-то во сне..."
      },
      {
        characters = { toPath(danil, "sleeping") },
        text = "Данил: Z-Z-Z-Z-z........"
      },
      {
        characters = { toPath(danil, "waking_up") },
        text = "Данил: Хммммммм...."
      }
    },
  },
  {
    background = toPath(images.backgrounds, "winter_mountain.jpg"),
    {
      {
        characters = { toPath(danil, "looking_up") },
        text = "Данил: А чё это вдруг резко зима началась?"
      },
      {
        characters = { toPath(shark_guy, "scarry"), toPath(danil, "looking_up") },
        text = "Страшный акуло-чел: БУГА-БУГА"
      },
      {
        characters = { toPath(shark_guy, "scarry"), toPath(danil, "disagree") },
        text = "Данил: Не, не страшна"
      },
      {
        characters = { toPath(shark_guy, "TwT"), toPath(danil, "disagree") },
        text = "Грустный акуло-чел: TwT"
      },
      {
        characters = { toPath(shark_guy, "angry"), toPath(danil, "disagree") },
        text = "Агрессивный акуло-чел: Не, ну хочешь я тебе тогда настоящую страшилку пакажу?"
      },
      {
        characters = { toPath(shark_guy, "angry"), toPath(danil, "agree") },
        text = "Данил: Го"
      },
      {
        characters = { toPath(shark_guy, "grinning"), toPath(danil, "agree") },
        text = "Подозрительный акуло-чел: Хехехеххехехеехехеххехехеххехеххех)))))))))))) >:) >:) >:) >:)"
      },
      {
        characters = { toPath(shark_guy, "grinning"), toPath(linus_torvalds, "based"), toPath(danil, "agree") },
        text = "Линус Торвальдс: ыыыыыыыыыыыыыыыы... Я прийду к тебе поздно ночью и поставлю линукс на твой компьютер"
      },
      {
        characters = { toPath(shark_guy, "grinning"), toPath(linus_torvalds, "based"), toPath(danil, "scared") },
        text = "Данил: АААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААААА"
      },
    }
  },
  {
    background = toPath(images.background, "black_screen"),
    {
      {
        characters = { },
        text = "*После такого Данил проснулся в холодном поту*"
      },
    }
  },
  {
    background = toPath(images.background, "danils_home1"),
    {
      {
        characters = { },
        text = "*Он быстро включил комп и успокоился, когда увидел, что там до сих пор стоит винда*"
      },
    }
  },
  {
    background = toPath(images.background, "danils_home2"),
    {
      {
        characters = { },
        text = "Данил: Фуууффф... Пронесло...."
      },
      {
        characters = { },
        text = "..."
      },
      {
        characters = { },
        text = "........."
      },
      {
        characters = { },
        text = "...: Тук-тук-тук..."
      },
      {
        characters = { toPath(linus_torwalds, "scarry") },
        text = "ПРИШЛО ВРЕМЯ СТАВИТЬ ЛИНУКС"
      }
    }
  },
  {
    background = toPath(images.background, "black_screen"),
    {
      {
        characters = { },
        text = "КОНЕЦ"
      },
      {
        characters = { toPath(linus_torwalds, "scarry") },
        text = "ВЫ СЛЕДУЮЩИЕ"
      }
    }
  }
}


local function getNovel()
  return novel
end

return getNovel
)lua";
