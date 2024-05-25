import { Component, EventEmitter, Input, Output } from "@angular/core";
import { Channel } from "../../models/Channel";

@Component({
    selector: 'channel',
    templateUrl: 'channel.component.html',
    styleUrls: ['channel.component.scss']
})

export class ChannelComponent {
    @Input() channel!: Channel

    @Output() channelClicked: EventEmitter<Channel> = new EventEmitter<Channel>();

    onChannelClicked(channel: Channel) {
        this.channelClicked.emit(channel);
    }
}