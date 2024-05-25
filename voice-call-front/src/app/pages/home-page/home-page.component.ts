import { Component } from "@angular/core";
import { Person } from "../../models/Person";
import { Channel } from "../../models/Channel";
import { ChannelType } from "../../enum/ChannelType.enum";
import { Router } from "@angular/router";

@Component({
    selector: 'home-page',
    templateUrl: 'home-page.component.html',
    styleUrls: ['home-page.component.scss']
})

export class HomePageComponent {

    constructor(private router: Router) {}

    personsInput: Person[] = [
        {id: 0, nick_name: 'Rub1', picture: 'https://cdn.discordapp.com/avatars/322089201455595530/fcbfbe222a844d19c97a029ab205903a.webp?size=128', text_form: true},
        {id: 1, nick_name: 'Rub1', picture: 'https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTEVfcgXdw6P_iFZzk9buMAyDVTwxcBjaw7I9SWghfxLQ&s', text_form: true},
        {id: 2, nick_name: 'Rub1', text_form: true}
    ]

    channelInput: Channel[] = [
        {type: ChannelType.Chat, name: 'General', persons: this.personsInput, messages: [], readonly: false},
        {type: ChannelType.Chat, name: 'Voice Stuff', persons: this.personsInput, messages: [], readonly: false},
        {type: ChannelType.Voice, name: 'Voice', persons: this.personsInput, messages: [], readonly: false},
    ]

    goToMessages(channel: Channel) {
        this.router.navigate([channel.name]);
    }
}