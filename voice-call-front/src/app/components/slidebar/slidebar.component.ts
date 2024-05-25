import { Component, EventEmitter, Input, Output } from "@angular/core";
import { Person } from "../../models/Person";
import { Channel } from "../../models/Channel";

@Component({
    selector: 'slidebar',
    templateUrl: 'slidebar.component.html',
    styleUrls: ['slidebar.component.scss']
})

export class SlideBarComponent {
    @Input() channels: Channel[] | null = null;

    @Input() persons: Person[] | null = null;

    @Output() channelClicked: EventEmitter<Channel> = new EventEmitter<Channel>();

    onChannelClicked(channel: Channel) {
        this.channelClicked.emit(channel);
    }
}